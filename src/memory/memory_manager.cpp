
#include "memory_manager.h"
#include "../graphics/includes/vga.h"
#include "debug.h"
#include "heap.h"
#include "task.h"
#include "macros.h"
#include "utils.h"
#include "kernel.h"


PageDirectory MemoryManager::kernelPageDirectory = 0;
bool MemoryManager::paging_enabled = false;


void MemoryManager::initMemoryManagement()
{
	paging_enabled = false;
	// On réserve cet espace pour le noyau (IDT, GDT, IVT et noyau)
	for(u32 addr=0x0; addr<0x100000; addr+=PAGE_SIZE)
		reserveFrame((Frame)addr);

	// On réserve cette espace pour le matériel, c'est la zone ou est mappée la mémoire video
	//for(u32 addr=0xA0000; addr<0x400000; addr+=PAGE_SIZE)
	//	reserveFrame((Frame)addr);

	// On récupère une frame pour le repertoire de page du noyau (elle sont initialisées)
	kernelPageDirectory = getFrame();
	// Puis une pour la premiere page de table
	PageTable kernelPageTable0 = getFrame();
	//PageTable kernelPageTable1 = getFrame();


 	// On initialise les répertoires et table pour mapper l'espace noyau
 	kernelPageDirectory[0] = (u32)kernelPageTable0 | PAGE_PRESENT | PAGE_WRITE | PAGE_USED;
 	//kernelPageDirectory[1] = (u32)kernelPageTable1 | PAGE_PRESENT | PAGE_WRITE | PAGE_USED;
 	for(int i=0; i<PAGE_TABLE_SIZE; i++)
 	{
 		kernelPageTable0[i] = (u32)PAGE_ADDR(i) | PAGE_PRESENT | PAGE_WRITE | PAGE_USED;
 		//kernelPageTable1[i] = (u32)PAGE_ADDR(i+PAGE_DIRECTORY_SIZE) | PAGE_PRESENT | PAGE_WRITE | PAGE_USED;
 	}

 	// Ces entrées permettent de modifier le repertoire ou la table de page en les faisant pointer vers eux-même
  	kernelPageDirectory[ITSELF_ENTRY] = (u32)kernelPageDirectory | PAGE_PRESENT | PAGE_WRITE | PAGE_USED;
 	kernelPageTable0[ITSELF_ENTRY] = (u32)kernelPageTable0 | PAGE_PRESENT | PAGE_WRITE | PAGE_USED;
 	//kernelPageTable1[ITSELF_ENTRY] = (u32)kernelPageTable1 | PAGE_PRESENT | PAGE_WRITE | PAGE_USED;;


 	// On active la pagination
	setCurrentPageDirectory(kernelPageDirectory);
	setPagingEnabled(true);
}


// On initialise à 0 la page
// priv est le privilège à mettre dans ITSELF
void MemoryManager::initPage(Page paddr, u16 priv)
{
	u32 *initPage;
	if(paging_enabled)
	{
		for(int i=0; i<PAGE_DIRECTORY_SIZE; i++)
			editPageEntry(paddr, i, 0, 0);
		editPageEntry(paddr, ITSELF_ENTRY, paddr, PAGE_USED | PAGE_WRITE | PAGE_PRESENT | (priv & PAGE_RIGHTS));
	}
	else
	{
		initPage = (u32*)paddr;

		for(int i=0; i<PAGE_DIRECTORY_SIZE; i++)
			initPage[i] = 0;

		initPage[ITSELF_ENTRY] = (u32)initPage | PAGE_USED | PAGE_WRITE | PAGE_PRESENT | (priv & PAGE_RIGHTS);
	}
}



void MemoryManager::setPagingEnabled(bool b)
{
	if(b)
	{
		asm("mov %cr0, %eax \n \
			orl $0x80000000, %eax \n \
			mov %eax, %cr0");
	}
	else
	{
		asm("mov %cr0, %eax \n \
			and $~0x80000000, %eax \n \
			mov %eax, %cr0");
	}
	paging_enabled = b;
}

void MemoryManager::setCurrentPageDirectory(PageDirectory pd)
{
	//bool prev_state = paging_enabled;
	asm("	movl %0, %%eax \n \
			movl %%eax, %%cr3":: "m"(pd));
	currentPageDirectory = pd;
}


u32 MemoryManager::getCurrentPageDirectoryEntry(u32 pd_entry)
{
	u32 *vaddr = (u32*)((ITSELF_ENTRY << 22) | (ITSELF_ENTRY << 12) | (pd_entry << 2)) ;
	return *vaddr;
}


u32 MemoryManager::getCurrentPageTableEntry(u32 pd_entry, u32 pt_entry)
{
	u32 *vaddr = (u32*)((ITSELF_ENTRY << 22) | (pd_entry  << 12) | (pt_entry << 2));
	return *vaddr;
}

void MemoryManager::editCurrentPageDirectoryEntry(u32 pd_entry, Frame paddr, u16 flags)
{
	u32 *vaddr = (u32*)((ITSELF_ENTRY << 22) | (ITSELF_ENTRY << 12) | (pd_entry << 2)) ;
	*vaddr = ((u32)paddr & 0xFFFFF000) | (0xFFF & flags);
}


void MemoryManager::editCurrentPageTableEntry(u32 pd_entry, u32 pt_entry,  Frame paddr, u16 flags)
{
	u32 *vaddr = (u32*)((ITSELF_ENTRY << 22) | (pd_entry << 12) | (pt_entry << 2));
	*vaddr = ((u32)(paddr) & 0xFFFFF000) | (0xFFF & flags);
}


void MemoryManager::editPageEntry(Page pg, u32 entry, Frame paddr, u16 flags)
{
	u32 *magic_page = (u32*)MAGIC_PAGE_ADDRESS;
	unloadAddr(magic_page);
	editCurrentPageTableEntry(PAGE_DIRECTORY_ENTRY(MAGIC_PAGE_ADDRESS), PAGE_TABLE_ENTRY(MAGIC_PAGE_ADDRESS),
	pg, PAGE_PRESENT | PAGE_WRITE | PAGE_USED);

	magic_page[entry] = ((u32)paddr & 0xFFFFF000) | (flags & 0xFFF);
	unloadAddr(magic_page);
}

void MemoryManager::copyPageEntry(Page from, Page to, u32 entry)
{
	u32 *magic_page = (u32*)MAGIC_PAGE_ADDRESS;
	unloadAddr(magic_page);
	unloadAddr(magic_page);
	editPageEntry(PAGE_DIRECTORY_ENTRY(MAGIC_PAGE_ADDRESS), PAGE_TABLE_ENTRY(MAGIC_PAGE_ADDRESS),
	to, PAGE_PRESENT | PAGE_WRITE | PAGE_USED);

	magic_page[entry] = (u32)from[entry];
	unloadAddr(magic_page);
}



bool MemoryManager::isPageFree(u32 *vaddr)
{
	u32 pd_entry = PAGE_DIRECTORY_ENTRY(vaddr);
	if(!IS_PAGE_USED(getCurrentPageDirectoryEntry(pd_entry)))
		return true;

	u32 pt_entry = PAGE_TABLE_ENTRY(vaddr);
	return !IS_PAGE_USED(getCurrentPageTableEntry(pd_entry, pt_entry));
}


// Cette fonction réserve une page en mémoire virtuelle. Elle l'alloue si ce n'est pas déjà fait.
// Elle renvoie vrai si la page n'existait pas et faux sinon
bool MemoryManager::reserveVirtualAddress(Page vaddr, u16 flags)
{
	u32 pd_entry = PAGE_DIRECTORY_ENTRY(vaddr);
	if(!IS_PAGE_USED(getCurrentPageDirectoryEntry(pd_entry))) // Si l'entrée de la table n'existe pas, on en reserve une
	{
		editCurrentPageDirectoryEntry(pd_entry, getFrame(), flags);
	}

	u32 pt_entry = PAGE_TABLE_ENTRY(vaddr);
	if(!IS_PAGE_USED(getCurrentPageTableEntry(pd_entry, pt_entry)))
	{
		editCurrentPageTableEntry(pd_entry, pt_entry, getFrame(), flags);
		return true;
	}

	return false;
}

void MemoryManager::freeVirtualAddress(Page vaddr)
{
	u32 pd_entry = PAGE_DIRECTORY_ENTRY(vaddr);
	u32 pt_entry = PAGE_TABLE_ENTRY(vaddr);
	Frame fr= (Frame)(getCurrentPageTableEntry(pd_entry, pt_entry) & 0xFFFFF000);
	freeFrame(fr);
	editCurrentPageTableEntry(pd_entry, pt_entry, 0, 0);
}


void MemoryManager::freeUserPageDirectory(PageDirectory pd)
{

}


PageDirectory MemoryManager::createTaskEnvironment(const int size_needed)
{
	//flushTLB();
	PageDirectory pd = getFrame();
	// On copie tout d'abord la partie basse qui se rapporte au noyau (0 à 0x400000)
	for(int i=0; i<PAGE_DIRECTORY_ENTRY(USER_TASK_ADDRESS); i++)
		pd[i] = kernelPageDirectory[i] | 3;

		//editPageEntry(pd, i, (Page)kernelPageDirectory[i], 3 );


	int s = size_needed;

	for(int j=PAGE_DIRECTORY_ENTRY(USER_TASK_ADDRESS); j<PAGE_DIRECTORY_SIZE-1 && s>0; j++)
	{
		Frame pt = getFrame();
		editPageEntry(pd, 256, pt, 7 | PAGE_USED);
		for(int k=0; k<PAGE_TABLE_SIZE-1 && s>0 ; k++)
		{
			Frame pg = getFrame();
			editPageEntry(pt, k, pg, 7 | PAGE_USED);
			s -= PAGE_SIZE;
		}
	}

	return pd;
}


void MemoryManager::loadTaskInMemory(const Task &t)
{
	setCurrentPageDirectory((PageDirectory)t.context.cr3);
	memcpy((char*)t.taskAddr, (char*)USER_TASK_ADDRESS, 0x1000);
	setCurrentPageDirectory(kernelPageDirectory);
}



bool MemoryManager::loadTaskPageDirectory(const Task &t)
{
	setCurrentPageDirectory((PageDirectory)t.context.cr3);
	return true;
}





