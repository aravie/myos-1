
#ifndef _MY_MEMORY_MANAGER_H
#define _MY_MEMORY_MANAGER_H

#include "memory_manager.h"
#include "singleton.h"

#define RAM_MAX_PAGE 0x10000

class MyMemoryManager : public MemoryManager , public Singleton<MyMemoryManager>
{
	friend class Singleton<MyMemoryManager>;
public:
	MyMemoryManager();
	// Renvoie l'adresse d'une frame physique libre
	virtual Frame getFrame();
	// On réserve une frame
	virtual void reserveFrame(Frame addr);
	// Libère une frame physique
	virtual void freeFrame(Frame addr);
	virtual bool isFrameFree(Frame addr);
private:
	unsigned char physicalMemoryMap[RAM_MAX_PAGE/8];
};


#endif
