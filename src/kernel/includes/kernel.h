
#ifndef _KERNEL_H
#define _KERNEL_H

#define KERNEL_HEAP_BOTTOM 	(void*)0x1000000
#define KERNEL_HEAP_TOP 		(void*)0x4000000


#include "heap.h"
//class Heap;

class Kernel
{
public:
	Kernel();

	void exec();

	static void *malloc(int size) { return heap.malloc((u32)size); }
	static void free(void *ptr) { heap.free(ptr); }
	static Heap heap;

private:
	// Boucle infinie qui correspond au processus du noyau
	void loop();

};




#endif

