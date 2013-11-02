
#ifndef _STDLIB_H
#define _STDLIB_H

#include "types.h"


#define KERNEL_MALLOC

#ifdef KERNEL_MALLOC
#include "kernel.h"



inline void* operator new(u32 size)
{
	return Kernel::malloc(size);
}

inline void* operator new[](u32 size)
{
	return Kernel::malloc(size);
}

inline void operator delete(void *ptr)
{
	Kernel::free(ptr);
}

inline void operator delete[](void *ptr)
{
	Kernel::free(ptr);
}
#endif  // KERNEL_MALLOC

void strcpy(char *src, char *dest, int len);


#endif
