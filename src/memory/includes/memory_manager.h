
#ifndef _MEMORY_MANAGER_H
#define _MEMORY_MANAGER_H

#include "types.h"
#include "singleton.h"

#define PAGE_USED 0x200
#define PAGE_WRITE 0x02
#define PAGE_PRESENT 0x01
#define PAGE_RIGHTS 0x04
#define ITSELF_ENTRY 0x3FF // C'est le champs qui fait pointer une page vers elle-même pour la modifier (Astuce)


#define MAGIC_PAGE_ADDRESS 0x2000


#define PAGE_DIRECTORY_SIZE 0x400
#define PAGE_TABLE_SIZE 0x400

#define PAGE_SIZE 4096
#define PAGE_4K 0
#define PAGE_4M 1

#define PAGE_ENTRY(paddr) ((u32)(paddr) >> 12)
#define PAGE_ADDR(entry) (Page)((u32)(entry) << 12)

#define PAGE_DIRECTORY_ENTRY(vaddr) (u16)(((u32)(vaddr) & 0xFFC00000) >> 22)
#define PAGE_TABLE_ENTRY(vaddr) (u16)(((u32)(vaddr) & 0x3FF000) >> 12)
#define PAGE_OFFSET(vaddr) (u16)(((u32)(vaddr) & 0xFFF))

#define IS_PAGE_USED(entry) ((entry) & PAGE_USED)

#define unloadAddr(vaddr) asm("mov %0, %%eax \n invlpg (%%eax)"::"m"((u32)vaddr))

#define flushTLB()  asm("mov %cr3, %eax \n mov %eax, %cr3")

typedef u32* PageDirectory;
typedef u32* PageTable;
typedef u32* Page;
typedef u32* Frame;


class Task;

class MemoryManager
{
	friend class TasksManager;
public:
	void initMemoryManagement();

	// On active la pagination
	void enablePaging() { setPagingEnabled(true); }
	// On désactive la pagination
	void disablePaging() { setPagingEnabled(false); }
	// On définit si la pagination est activée
	void setPagingEnabled(bool b);

	// Renvoie l'adresse d'une frame physique libre
	virtual Frame getFrame() = 0;
	// On réserve une frame
	virtual void reserveFrame(Frame addr) = 0;
	// Libère une frame physique
	virtual void freeFrame(Frame addr) = 0;
	virtual bool isFrameFree(Frame addr) = 0;

	// On accède ou on modifie une entrée d'une page de table ou d'un rep. de page
	u32 getCurrentPageDirectoryEntry(u32 pd_entry);
	u32 getCurrentPageTableEntry(u32 pd_entry, u32 pt_entry);
	static void editCurrentPageDirectoryEntry(u32 entry, Frame paddr, u16 flags);
	static void editCurrentPageTableEntry(u32 pd_entry, u32 pt_entry,  Frame paddr, u16 flags);

	static void editPageEntry(Page pg, u32 entry, Frame paddr, u16 flags);
	static void copyPageEntry(Page from, Page to, u32 entry);

	// Regarde si l'adresse virtuelle est déjà réservé, si ce n'est pas le cas, il la réserve
	bool reserveVirtualAddress(Page vaddr, u16 flags);
	//bool reserveVirtualAddress(PageDirectory pd, Page vaddr, u16 flags);
	void freeVirtualAddress(Page vaddr);

	void freeUserPageDirectory(PageDirectory pd);

	// renvoie vrai si la page qui correspond à l'adresse virtuelle vaddr est libre
	bool isPageFree(Page vaddr);

	// On définit le repertoire de page courant (CR2)
	void setCurrentPageDirectory(PageDirectory pd);

	PageDirectory createTaskEnvironment(const int size_needed);
	void loadTaskInMemory(const Task &t);
	bool loadTaskPageDirectory(const Task &t);

	static void initPage(Page pd, u16 priv);

private:
	// Repertoire de page du noyau
	static PageDirectory kernelPageDirectory;
	PageDirectory currentPageDirectory;
	static bool paging_enabled;


	char page_size;
};




#endif
