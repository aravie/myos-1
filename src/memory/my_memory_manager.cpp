
#include "memory_manager.h"
#include "my_memory_manager.h"
#include "../libs/includes/utils.h"
#include "debug.h"

MyMemoryManager::MyMemoryManager()
{
	// on initialise la map
	for(int i=0; i<RAM_MAX_PAGE/8; i++)
	{
		physicalMemoryMap[i] = 0;
	}
}


// Renvoie l'adresse d'une frame physique libre
Frame MyMemoryManager::getFrame()
{
	bool terminated = false;
	int octet=0;
	unsigned char bits;

	while(!terminated)
	{
		if(octet*8>=RAM_MAX_PAGE)
			terminated = true;

		if(physicalMemoryMap[octet] == 0xFF)
			octet++;
		else
		{
			bits = 1;
			for(int i=0; i<8 && !terminated; i++)
			{
				if(!((bits << i) & physicalMemoryMap[octet]))
				{
					Frame addr = (Frame)PAGE_ADDR(8*octet+i);
					reserveFrame(addr);
					initPage(addr, 3);
					return addr;
				}
			}
		}
	}
	return (Frame)-1; // On renvoie une erreur car la fonction n'a pas trouvé de page libre
}

// On réserve une frame
void MyMemoryManager::reserveFrame(Frame addr)
{
	u32 entry = PAGE_ENTRY(addr);
	u32 octet = entry >> 3;  // On divise par 8
	unsigned char bit = entry & 0x07; // On récupère le reste de la division par 8

	/*if(addr == (Frame)0xA0000)
	{
		debug((unsigned char)octet);
		debug((unsigned char)bit);
	}*/


	unsigned char mask = 1 << bit;
	// On place le bit correspondant à la page à 1
	physicalMemoryMap[octet] |= mask;
}

// Libère une frame physique
void MyMemoryManager::freeFrame(Frame addr)
{
	int entry = PAGE_ENTRY(addr);
	int octet = entry >> 3;  // On divise par 8
	int bit = entry & 0x07; // On récupère le reste de la diviosn par 8

	char mask = 1 << bit;
	// On place le bit correspondant à la page à 0
	physicalMemoryMap[octet] &= !mask;
}


// Dit si une frame associée à une adresse est libre ou non
bool MyMemoryManager::isFrameFree(Frame addr)
{
	u32 entry = PAGE_ENTRY(addr);
	u32 octet = entry >> 3;  // On divise par 8
	unsigned char bit = entry & 0x07; // On récupère le reste de la division par 8

	return !(physicalMemoryMap[octet] & (1 << bit));
}
