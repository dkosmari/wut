#include <coreinit/memdefaultheap.h>
#include <coreinit/memexpheap.h>
#include <coreinit/memorymap.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>

void
__init_wut_malloc(void)
{
}

void
__fini_wut_malloc(void)
{
}

void *
_malloc_r(struct _reent *r, size_t size)
{
   void *ptr = MEMAllocFromDefaultHeapEx(size, 0x40);
   if (!ptr) {
      r->_errno = ENOMEM;
   }
   return ptr;
}

void
_free_r(struct _reent *r, void *ptr)
{
   if (ptr) {
      MEMFreeToDefaultHeap(ptr);
   }
}

void *
_realloc_r(struct _reent *r, void *ptr, size_t size)
{
   void *new_ptr = MEMAllocFromDefaultHeapEx(size, 0x40);
   if (!new_ptr) {
      r->_errno = ENOMEM;
      return new_ptr;
   }

   if (ptr) {
      size_t old_size = MEMGetSizeForMBlockExpHeap(ptr);
      memcpy(new_ptr, ptr, old_size <= size ? old_size : size);
      MEMFreeToDefaultHeap(ptr);
   }
   return new_ptr;
}

void *
_calloc_r(struct _reent *r, size_t num, size_t size)
{
   void *ptr = MEMAllocFromDefaultHeapEx(num * size, 0x40);
   if (ptr) {
      memset(ptr, 0, num * size);
   } else {
      r->_errno = ENOMEM;
   }

   return ptr;
}

void *
_memalign_r(struct _reent *r, size_t align, size_t size)
{
   void *ptr = MEMAllocFromDefaultHeapEx((size + align - 1) & ~(align - 1), align);
   if (!ptr) {
      r->_errno = ENOMEM;
   }
   return ptr;
}

struct mallinfo
_mallinfo_r(struct _reent *r)
{
   struct mallinfo info = {0};
   return info;
}

void
_malloc_stats_r(struct _reent *r)
{
}

int
_mallopt_r(struct _reent *r, int param, int value)
{
   return 0;
}

size_t
_malloc_usable_size_r(struct _reent *r, void *ptr)
{
   return MEMGetSizeForMBlockExpHeap(ptr);
}

void *
_valloc_r(struct _reent *r, size_t size)
{
   void *ptr = MEMAllocFromDefaultHeapEx(size, OS_PAGE_SIZE);
   if (!ptr) {
      r->_errno = ENOMEM;
   }
   return ptr;
}

void *
_pvalloc_r(struct _reent *r, size_t size)
{
   void *ptr = MEMAllocFromDefaultHeapEx((size + (OS_PAGE_SIZE - 1)) & ~(OS_PAGE_SIZE - 1), OS_PAGE_SIZE);
   if (!ptr) {
      r->_errno = ENOMEM;
   }
   return ptr;
}

int
_malloc_trim_r(struct _reent *r, size_t pad)
{
   return 0;
}
