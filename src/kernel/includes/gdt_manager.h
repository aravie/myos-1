
#ifndef _GDT_MANAGER_H
#define _GDT_MANAGER_H

#include "types.h"
#include "singleton.h"
#include "tasks_manager.h"


#define GDT_SIZE 0xFF
#define GDT_BASE	0x500
#define GDT_ENTRY_SIZE 8

struct GDTDescriptor
{
	u16 limit0_15;
	u16 base0_15;
	u8 base16_23;
	u8 access_byte;
	u8 limit16_19 : 4; // on ne prend que 4 bits de mémoire
	u8 flags : 4;
	u8 base24_31;
} __attribute__ ((packed));

struct GDTPtr
{
	u16 limit;
	u32 base;
} __attribute__ ((packed));


class GDTManager : public Singleton<GDTManager>
{
	friend class Singleton<GDTManager>;
	friend void TasksManager::init();
public:
	void initGDT();


private:
	void createDescriptor(u32 base, u32 limit, u8 access, u8 flags, GDTDescriptor *desc);

};

#endif
