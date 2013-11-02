
#ifndef _IDT_MANAGER_H
#define _IDT_MANAGER_H

#include "types.h"
#include "singleton.h"

#define INTGATE 0x8E00
#define TRAPGATE 0xEF00


#define IDT_SIZE 0xFF
#define IDT_BASE 0xD00;
#define IDT_ENTRY_SIZE 8

struct IDTDescriptor
{
	u16 offset0_15;
	u16 segment_selector;
	u16 flags;
	u16 offset16_31;
} __attribute__((packed));

struct IDTPtr
{
	u16 limit;
	u32 base;
} __attribute__((packed));

class IDTManager : public Singleton<IDTManager>
{
	friend class Singleton<IDTManager>;
public:
	void initIDT();
	void assignRoutine(u16 segment, u32 offset, u8 interrupt_num);

protected:
	void createDescriptor(u16 segment, u32 offset, u16 flags, IDTDescriptor *desc);

};

void configurePIC();
void resetPIC();


#endif

