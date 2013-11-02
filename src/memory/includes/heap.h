
#ifndef _HEAP_H
#define _HEAP_H

#include "singleton.h"
#include "types.h"
#include "memory_manager.h"


#define HEADER_SIZE sizeof(HeapSpaceHeader)
#define MIN_SIZE 4 // On définit une taille minimale en octet pour laquelle on fera un découpage
// Si il reste moins de 4 octets plus la taille de l'entête, on ne decoupe pas

#define IS_ADDR_VALID(addr)  (addr<(char*)heapTop && addr>(char*)heapBottom)

struct HeapSpaceHeader
{
	u8 used:1;
	u32 size:31;
	HeapSpaceHeader *prec; // On sauvegarde aussi le precédent pour pouvoir libérer plus facilement la mémoire et regrouper des blocs
} __attribute__((packed));



// Attention ! : Pour l'utiliser, il faut que la pagination soit activée...

class Heap
{
public:
	enum Level {USER, KERNEL};

	void initHeap(Level l, const void *bottom, const void *top);
	void initHeap2(const void *bottom, const void *top);

	// Etend l'espace du tas en allouant 'size' pages en plus
	void extendHeap(int size);
	// On réduit le tas de 'size' pages
	void reduceHeap(int size);

	// Coupe le bloc libre si c'est nécessaire
	void *cutSpace(HeapSpaceHeader *header, u32 size);

	// Alloue size octets de mémoire, dynamiquement
	void *malloc(u32 size);
	void free(void *ptr);

	int getHeapTop() const { return (int)heapTop; }

	void _printHeapState();

private:
	//HeapSpaceHeader *firstFree; // Cette variable va nou servir à définir le premier élèment libre dans le tas
	// Cela évite de parcourir toute la heap si le premier élément libre est au milieu du tas (optimisation)

	u16 getPageFlags();

	Level lvl;

	HeapSpaceHeader *heapTop;
	HeapSpaceHeader *heapBottom;

	// Pointe sur le dernier espace allouable actuellement présent en mémoire
	HeapSpaceHeader *heapEnd;
};

#endif
