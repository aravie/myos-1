
#include "heap.h"
#include "my_memory_manager.h"
#include "memory_manager.h"
#include "singleton.h"
#include "utils.h"
#include "debug.h"
#include "vga.h"

extern Vga screen;


u16 Heap::getPageFlags()
{
	u16 flags = 0;
	if(lvl == USER)
		flags = 7 | PAGE_USED;
	else if(lvl == KERNEL)
	{
		flags = 3 | PAGE_USED;
	}
	return flags;
}


void Heap::initHeap(Heap::Level lv, const void *bottom, const void *top)
{
	lvl = lv;
	heapTop = (HeapSpaceHeader*)top;
	heapBottom =(HeapSpaceHeader*)bottom;

	MyMemoryManager::getInstance()->reserveVirtualAddress((Page)heapBottom, getPageFlags());


	HeapSpaceHeader* heapHeader = (HeapSpaceHeader*)heapBottom;
	heapHeader->used = 0;
	heapHeader->size = PAGE_SIZE-HEADER_SIZE;
	heapHeader->prec = 0;

	heapEnd=heapHeader;
}

void Heap::initHeap2(const void *bottom, const void *top)
{
	heapTop = (HeapSpaceHeader*)top;
	heapBottom =(HeapSpaceHeader*)bottom;

	HeapSpaceHeader* heapHeader = (HeapSpaceHeader*)heapBottom;
	heapHeader->used = 0;
	heapHeader->size = PAGE_SIZE-HEADER_SIZE;
	heapHeader->prec = 0;

	heapEnd=heapHeader;
}


void Heap::extendHeap(int size)
{
	char *addr = (char*)heapEnd + HEADER_SIZE+heapEnd->size;
	for(int i=0; i<size; i++)
	{
		MyMemoryManager::getInstance()->reserveVirtualAddress((Page)((char*)addr+i*PAGE_SIZE), getPageFlags());
	}
}

void Heap::reduceHeap(int size)
{
	char *addr = (char*)heapEnd + HEADER_SIZE + heapEnd->size;
	for(int i=0; i<size; i++)
	{
		MyMemoryManager::getInstance()->freeVirtualAddress((Page)((char*)addr+i*PAGE_SIZE));
	}
}



// On passe en parametre l'adresse d'un bloc qui a une taille plus gde que la taille à allouer
// et on regarde si on peut le découper
void *Heap::cutSpace(HeapSpaceHeader *header, u32 size)
{
	if(size < MIN_SIZE)
		size = MIN_SIZE;
	// on l'alloue
	if(header->size-size >= HEADER_SIZE+MIN_SIZE) // Si il reste assez de place pour un découpage, on découpe
	{
		//printAddr(header);
		u32 big_size = header->size; // Variable temporaire pour calculer la taille du bloc que l'on crée
		header->used = 1;
		header->size = size;
		HeapSpaceHeader *next_header = (HeapSpaceHeader*)((char*)header + HEADER_SIZE + size);
		//printAddr(next_header);

		// on ne modifie pas prec car le précédent est tjrs le même
		next_header->used = 0;
		next_header->size = big_size - size - HEADER_SIZE;
		next_header->prec = header;
		/*if(next_header < firstFree)
			firstFree = next_header;*/
		if(header == heapEnd)
			heapEnd = next_header;
	}
	else
	{
		header->used = 1;
	}
	return (void*)((u32)header+HEADER_SIZE);
}


void *Heap::malloc(u32 size)
{
	bool end = false;

	if(size <= 0)
		return 0;

	HeapSpaceHeader *header = (HeapSpaceHeader*)heapBottom;
	while(!end)
	{
		// Si le bloc est bon
		if(!header->used && header->size >= size)
		{
			return cutSpace(header, size);
		}

		if(header < heapEnd)
			header = (HeapSpaceHeader*)((u32)header + header->size+HEADER_SIZE);
		else if(header == heapEnd)
		{
			end = true;
		}
	}

	// Si on peut encore étendre le tas, header contient le plus grand bloc du tas
	if((char*)heapEnd + HEADER_SIZE + heapEnd->size < (char*)heapTop)
	{ // Alors on étend le tas en allouant de la mémoire virtuelle
		u32 size_asked = size; // Taille demandée en octets
		HeapSpaceHeader* next;

		if(!header->used)
		{
			size_asked -= header->size; // On retire les octets qui sont déjà libre à la fin du tas
		}
		short nb_pages = (size_asked/PAGE_SIZE)+1;
		//debug((u32)nb_pages);
		extendHeap(nb_pages); // On étend le tas du nb de pages qu'il faut

		if(!header->used)
		{
			header->size += nb_pages*PAGE_SIZE;
			next = header;
		}
		else
		{
			next = (HeapSpaceHeader*)((char*)header + HEADER_SIZE + header->size);
			next->size = nb_pages*PAGE_SIZE - HEADER_SIZE;
			next->used = false;
			next->prec = header;
			heapEnd = next;
		}
		return cutSpace(next, size);
	}

	return (void*)-1;
}

void Heap::free(void *ptr)
{
	HeapSpaceHeader *header = (HeapSpaceHeader *)((char*)ptr - HEADER_SIZE);
	header->used = 0;

	// On regroupe le bloc précédent si c'est possible
	if(header > (HeapSpaceHeader*)heapBottom)
	{
		HeapSpaceHeader *prec = header->prec;
		// Si le bloc précédent n'est pas utilisé
		if(!prec->used)
		{
			prec->size += header->size + HEADER_SIZE;
			/*if(firstFree > prec)
				firstFree = prec;*/
			if(header == heapEnd)
			{
				// Si le bloc libéré n'est pas le premier et qu'il est en haut du tas
				// et si le precedent est libre, alors on les scinde et et le prec devient le haut du tas
					heapEnd = prec;
			}
			else
			{
				// S'il n'était pas en haut du tas, on va à l'élément suivant et on fait pointé le précédent *
				// vers prec
				HeapSpaceHeader *next = (HeapSpaceHeader*)((u32)header + header->size + HEADER_SIZE);
				next->prec = prec;
			}
			header = prec;
		}
	}

	// On regroupe le bloc suivant si c'est possible
	if(header < heapEnd)
	{
		HeapSpaceHeader *next = (HeapSpaceHeader*)((char*)header + header->size + HEADER_SIZE);
		if(!next->used)
		{
			header->size += next->size + HEADER_SIZE;
			if(next == heapEnd)
			{
				heapEnd = header;
			}else
			{
				next = (HeapSpaceHeader*)((u32)header + header->size + HEADER_SIZE);  // next next
				next->prec = header; // On met le pointeur du suivant
			}
		}
	}

	// Si header est le dernier bloc et qu'il prend plus d'une page alors on peut réduire le tas pour faire de la place en mémoire.
	if(header==heapEnd && header->size + MIN_SIZE  >= PAGE_SIZE)
	{
		unsigned short nb_pages = (header->size/PAGE_SIZE);
		reduceHeap(nb_pages);
		header->size -= (u32)(nb_pages * PAGE_SIZE);
	}
}

void Heap::_printHeapState()
{
	HeapSpaceHeader *header = (HeapSpaceHeader*)heapBottom, *prec;

	Screen->print("Heap State :\n");
	do
	{
		Screen->printAddress((char*)header);
		Screen->print(" : used=");
		(header->used) ? Screen->print("yes, size=") : Screen->print("no, size=");
		Screen->putNb(header->size);
		Screen->print(", prec=");
		Screen->printAddress(header->prec);
		Screen->print("\n");

		prec = header;
		header = (HeapSpaceHeader*)((u32)header + header->size + HEADER_SIZE);
	} while(prec != heapEnd && prec->size != 0);
}

