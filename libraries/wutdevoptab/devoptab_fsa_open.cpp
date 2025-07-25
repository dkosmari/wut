#include <mutex>
#include "devoptab_fsa.h"

// Extended "magic" value that allows opening files with FS_OPEN_FLAG_UNENCRYPTED in underlying FSOpenFileEx() call similar to O_DIRECTORY
#ifndef O_UNENCRYPTED
#define O_UNENCRYPTED 0x4000000
#endif

int
__wut_fsa_open(struct _reent *r,
               void *fileStruct,
               const char *path,
               int flags,
               int mode)
{
   FSAFileHandle fd;
   FSError status;
   const char *fsMode;
   __wut_fsa_file_t *file;
   __wut_fsa_device_t *deviceData;

   if (!fileStruct || !path) {
      r->_errno = EINVAL;
      return -1;
   }

   bool createFileIfNotFound = false;
   bool failIfFileNotFound   = false;
   // Map flags to open modes
   int commonFlagMask        = O_CREAT | O_TRUNC | O_APPEND;
   if (((flags & O_ACCMODE) == O_RDONLY) && !(flags & commonFlagMask)) {
      fsMode = "r";
   } else if (((flags & O_ACCMODE) == O_RDWR) && !(flags & commonFlagMask)) {
      fsMode = "r+";
   } else if (((flags & O_ACCMODE) == O_WRONLY) && ((flags & commonFlagMask) == (O_CREAT | O_TRUNC))) {
      fsMode = "w";
   } else if (((flags & O_ACCMODE) == O_RDWR) && ((flags & commonFlagMask) == (O_CREAT | O_TRUNC))) {
      fsMode = "w+";
   } else if (((flags & O_ACCMODE) == O_WRONLY) && ((flags & commonFlagMask) == O_CREAT) && (flags & O_EXCL) == O_EXCL) {
      // if O_EXCL is set, we don't need O_TRUNC
      fsMode = "w";
   } else if (((flags & O_ACCMODE) == O_RDWR) && ((flags & commonFlagMask) == O_CREAT) && (flags & O_EXCL) == O_EXCL) {
      // if O_EXCL is set, we don't need O_TRUNC
      fsMode = "w+";
   } else if (((flags & O_ACCMODE) == O_WRONLY) && ((flags & commonFlagMask) == (O_CREAT | O_APPEND))) {
      fsMode = "a";
   } else if (((flags & O_ACCMODE) == O_RDWR) && ((flags & commonFlagMask) == (O_CREAT | O_APPEND))) {
      fsMode = "a+";
   } else if (((flags & O_ACCMODE) == O_WRONLY) && ((flags & commonFlagMask) == (O_CREAT))) {
      // Cafe OS doesn't have a matching mode for this, so we have to be creative and create the file.
      createFileIfNotFound = true;
      // It's not possible to open a file with write only mode which doesn't truncate the file
      // Technically we could read from the file, but our read implementation is blocking this.
      fsMode               = "r+";
   } else if (((flags & O_ACCMODE) == O_RDWR) && ((flags & commonFlagMask) == (O_CREAT))) {
      // Cafe OS doesn't have a matching mode for this, so we have to be creative and create the file.
      createFileIfNotFound = true;
      fsMode               = "r+";
   } else if (((flags & O_ACCMODE) == O_WRONLY) && ((flags & commonFlagMask) == (O_APPEND))) {
      // Cafe OS doesn't have a matching mode for this, so we have to check if the file exists.
      failIfFileNotFound = true;
      fsMode             = "a";
   } else if (((flags & O_ACCMODE) == O_WRONLY) && ((flags & commonFlagMask) == (O_TRUNC))) {
      // As above
      failIfFileNotFound = true;
      fsMode             = "w";
   } else {
      r->_errno = EINVAL;
      return -1;
   }

   char *fixedPath = __wut_fsa_fixpath(r, path);
   if (!fixedPath) {
      r->_errno = ENOMEM;
      return -1;
   }


   file       = (__wut_fsa_file_t *)fileStruct;
   deviceData = (__wut_fsa_device_t *)r->deviceData;

   if (snprintf(file->fullPath, sizeof(file->fullPath), "%s", fixedPath) >= (int)sizeof(file->fullPath)) {
      WUT_DEBUG_REPORT("__wut_fsa_open: snprintf result was truncated\n");
   }
   free(fixedPath);

   // Prepare flags
   FSOpenFileFlags openFlags = (flags & O_UNENCRYPTED) ? FS_OPEN_FLAG_UNENCRYPTED : FS_OPEN_FLAG_NONE;
   FSMode translatedMode     = __wut_fsa_translate_permission_mode(mode);
   uint32_t preAllocSize     = 0;

   // Init mutex and lock
   file->mutex.init(file->fullPath);
   std::scoped_lock lock(file->mutex);

   if (createFileIfNotFound || failIfFileNotFound || (flags & (O_EXCL | O_CREAT)) == (O_EXCL | O_CREAT)) {
      // Check if file exists
      FSAStat stat;
      status = FSAGetStat(deviceData->clientHandle, file->fullPath, &stat);
      if (status == FS_ERROR_NOT_FOUND) {
         if (createFileIfNotFound) { // Create new file if needed
            status = FSAOpenFileEx(deviceData->clientHandle, file->fullPath, "w", translatedMode,
                                   openFlags, preAllocSize, &fd);
            if (status == FS_ERROR_OK) {
               if (FSACloseFile(deviceData->clientHandle, fd) != FS_ERROR_OK) {
                  WUT_DEBUG_REPORT("FSACloseFile(0x%08X, 0x%08X) (%s) failed: %s\n",
                                   deviceData->clientHandle, fd, file->fullPath, FSAGetStatusStr(status));
               }
               fd = -1;
            } else {
               WUT_DEBUG_REPORT("FSAOpenFileEx(0x%08X, %s, %s, 0x%X, 0x%08X, 0x%08X, %p) failed: %s\n",
                                deviceData->clientHandle, file->fullPath, "w", translatedMode, openFlags, preAllocSize, &fd,
                                FSAGetStatusStr(status));
               r->_errno = __wut_fsa_translate_error(status);
               return -1;
            }
         } else if (failIfFileNotFound) { // Return an error if we don't we create new files
            r->_errno = __wut_fsa_translate_error(status);
            return -1;
         }
      } else if (status == FS_ERROR_OK) {
         // If O_CREAT and O_EXCL are set, open() shall fail if the file exists.
         if ((flags & (O_EXCL | O_CREAT)) == (O_EXCL | O_CREAT)) {
            r->_errno = EEXIST;
            return -1;
         }
      }
   }

   status = FSAOpenFileEx(deviceData->clientHandle, file->fullPath, fsMode, translatedMode, openFlags, preAllocSize, &fd);
   if (status < 0) {
      if (status != FS_ERROR_NOT_FOUND) {
         WUT_DEBUG_REPORT("FSAOpenFileEx(0x%08X, %s, %s, 0x%X, 0x%08X, 0x%08X, %p) failed: %s\n",
                          deviceData->clientHandle, file->fullPath, fsMode, translatedMode, openFlags, preAllocSize, &fd,
                          FSAGetStatusStr(status));
      }
      r->_errno = __wut_fsa_translate_error(status);
      return -1;
   }

   file->fd     = fd;
   file->flags  = (flags & (O_ACCMODE | O_APPEND | O_SYNC));
   // Is always 0, even if O_APPEND is set.
   file->offset = 0;

   if (flags & O_APPEND) {
      FSAStat stat;
      status = FSAGetStatFile(deviceData->clientHandle, fd, &stat);
      if (status < 0) {
         WUT_DEBUG_REPORT("FSAGetStatFile(0x%08X, 0x%08X, %p) (%s) failed: %s\n",
                          deviceData->clientHandle, fd, &stat, file->fullPath, FSAGetStatusStr(status));

         r->_errno = __wut_fsa_translate_error(status);
         if (FSACloseFile(deviceData->clientHandle, fd) < 0) {
            WUT_DEBUG_REPORT("FSACloseFile(0x%08X, 0x%08X) (%s) failed: %s\n",
                             deviceData->clientHandle, fd, file->fullPath, FSAGetStatusStr(status));
         }
         return -1;
      }
      file->appendOffset = stat.size;
   }
   return 0;
}
