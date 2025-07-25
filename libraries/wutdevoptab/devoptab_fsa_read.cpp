#include <mutex>
#include <sys/param.h>
#include "devoptab_fsa.h"

ssize_t
__wut_fsa_read(struct _reent *r, void *fd, char *ptr, size_t len)
{
   FSError status;
   __wut_fsa_file_t *file;
   __wut_fsa_device_t *deviceData;
   if (!fd || !ptr) {
      r->_errno = EINVAL;
      return -1;
   }

   // Check that the file was opened with read access
   file = (__wut_fsa_file_t *)fd;
   if ((file->flags & O_ACCMODE) == O_WRONLY) {
      r->_errno = EBADF;
      return -1;
   }

   // cache-aligned, cache-line-sized
   __attribute__((aligned(0x40))) uint8_t alignedBuffer[0x40];

   deviceData = (__wut_fsa_device_t *)r->deviceData;

   std::scoped_lock lock(file->mutex);

   size_t bytesRead = 0;
   while (bytesRead < len) {
      // only use input buffer if cache-aligned and read size is a multiple of cache line size
      // otherwise read into alignedBuffer
      uint8_t *tmp = (uint8_t *)ptr;
      size_t size  = len - bytesRead;

      if (size < 0x40) {
         // read partial cache-line back-end
         tmp = alignedBuffer;
      } else if ((uintptr_t)ptr & 0x3F) {
         // read partial cache-line front-end
         tmp  = alignedBuffer;
         size = MIN(size, 0x40 - ((uintptr_t)ptr & 0x3F));
      } else {
         // read whole cache lines
         size &= ~0x3F;
      }

      // Limit each request to 1 MiB
      if (size > 0x100000) {
         size = 0x100000;
      }

      status = FSAReadFile(deviceData->clientHandle, tmp, 1, size, file->fd, 0);

      if (status < 0) {
         WUT_DEBUG_REPORT("FSAReadFile(0x%08X, %p, 1, 0x%08X, 0x%08X, 0) (%s) failed: %s\n",
                          deviceData->clientHandle, tmp, size, file->fd, file->fullPath, FSAGetStatusStr(status));

         if (bytesRead != 0) {
            return bytesRead; // error after partial read
         }

         r->_errno = __wut_fsa_translate_error(status);
         return -1;
      }

      if (tmp == alignedBuffer) {
         memcpy(ptr, alignedBuffer, status);
      }

      file->offset += status;
      bytesRead += status;
      ptr += status;

      if ((size_t)status != size) {
         return bytesRead; // partial read
      }
   }

   return bytesRead;
}
