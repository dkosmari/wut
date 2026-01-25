#include "wut_socket.h"

static
int
f_get_fd(int sockfd)
{
   /* FD_CLOEXEC flag is always set. */
   (void)sockfd;
   return FD_CLOEXEC;
}

static
int
f_set_fd(int sockfd,
         int flags)
{
   /* FD_CLOEXEC cannot be cleared, but this always succeeds. */
   return 0;
}

static
int
f_get_fl(int sockfd)
{
   int flags = 0;
   int nonblock;
   socklen_t nonblock_len = sizeof nonblock;

   int rc = RPLWRAP(getsockopt)(sockfd,
                                SOL_SOCKET,
                                SO_NONBLOCK,
                                &nonblock,
                                &nonblock_len);
   if (rc == -1)
      return __wut_get_nsysnet_result(NULL, rc);

   if (nonblock)
      flags |= O_NONBLOCK;

   return flags;
}

static
int
f_set_fl(int sockfd,
         int flags)
{
   int nonblock = !!(flags & O_NONBLOCK);
   socklen_t nonblock_len = sizeof nonblock;
   int rc = RPLWRAP(setsockopt)(sockfd,
                                SOL_SOCKET,
                                SO_NONBLOCK,
                                &nonblock,
                                nonblock_len);
   return __wut_get_nsysnet_result(NULL, rc);
}

int
fcntl(int fd,
      int cmd,
      ...)
{
   va_list args;
   int flags;
   int sockfd = __wut_get_nsysnet_fd(fd);
   if (sockfd == -1) {
      errno = EBADF;
      return -1;
   }

   switch (cmd) {

      case F_GETFD:
         return f_get_fd(sockfd);

      case F_SETFD:
         va_start(args, cmd);
         flags = va_arg(args, int);
         va_end(args);
         return f_set_fd(sockfd, flags);

      case F_GETFL:
         return f_get_fl(sockfd);

      case F_SETFL:
         va_start(args, cmd);
         flags = va_arg(args, int);
         va_end(args);
         return f_set_fl(sockfd, flags);

      case F_DUPFD:
      case F_GETLK:
      case F_SETLK:
      case F_SETLKW:
#if __GNU_VISIBLE || __POSIX_VISIBLE >= 202405
      case F_OFD_GETLK:
      case F_OFD_SETLK:
      case F_OFD_SETLKW:
#endif
         errno = EINVAL;
         return -1;

#if __BSD_VISIBLE || __POSIX_VISIBLE >= 200112
      case F_GETOWN:
      case F_SETOWN:
#endif
      default:
         errno = EOPNOTSUPP;
         return -1;
   }
}
