
#include "idt_manager.h"	
#include "macros.h"
#include "../interrupts/includes/isr.h"
#include "utils.h"
#include "../graphics/includes/vga.h"

IDTPtr idtptr;



void IDTManager::initIDT()
{
	IDTDescriptor idt[IDT_SIZE];
	cli;
	for(u32 i=0; i<IDT_SIZE; i++)
	{
		createDescriptor(0x8, (u32) _default_interrupt, INTGATE, &idt[i]);
	}

	createDescriptor(0x8, (u32) _general_protection_fault, INTGATE, &idt[13]);
	createDescriptor(0x8, (u32) _page_fault, INTGATE, &idt[14]); // Page FAULT

	createDescriptor(0x8, (u32) _irq_0, INTGATE, &idt[0x20]); // Clock
	createDescriptor(0x8, (u32) _irq_1, INTGATE, &idt[0x21]); // Keyboard

	createDescriptor(0x8, (u32)_system_calls, INTGATE | 0x6000, &idt[0x30]); // System calls
	//createDescriptor(0x8, (u32) irq_12, INTGATE, &idt[37]); // Mouse PS/2
	//createDescriptor(0x8, (u32) irq_12, INTGATE, &idt[0x74]); // Mouse PS/2

	createDescriptor(0x8, (u32) _irq_14, INTGATE, &idt[0x76]); // Bus primaire
	createDescriptor(0x8, (u32) _irq_15, INTGATE, &idt[0x77]); // Bus secondaire

	idtptr.limit = IDT_SIZE * IDT_ENTRY_SIZE;
	idtptr.base = IDT_BASE;

	memcpy((char*)idt, (char*)idtptr.base, idtptr.limit);

	__asm__("lidtl (idtptr)");
	
}

void IDTManager::createDescriptor(u16 segment, u32 offset, u16 flags, IDTDescriptor *desc)
{
	desc->segment_selector = segment;
	desc->offset0_15 = offset & 0xFFFF;
	desc->offset16_31 = (offset & 0xFFFF0000) >> 16;
	desc->flags = flags;
}


void assignRoutine(u16 segment, u32 offset, u8 interrupt_num)
{
	IDTDescriptor *idt = (IDTDescriptor*)idtptr.base;
	idt[interrupt_num].segment_selector = segment;
	idt[interrupt_num].offset0_15 = offset & 0xFFFF;
	idt[interrupt_num].offset16_31 = (offset & 0xFFFF0000) >> 16;
}



void configurePIC()
{
	/* Initialisation de ICW1 */
	outb(0x20, 0x11);
	outb(0xA0, 0x11);

	/* Initialisation de ICW2 */
	outb(0x21, 0x20);	/* vecteur de depart = 32 */
	outb(0xA1, 0x70);	/* vecteur de depart = 96 */

	/* Initialisation de ICW3 */
	outb(0x21, 0x04);
	outb(0xA1, 0x02);

	/* Initialisation de ICW4 */
	outb(0x21, 0x01);
	outb(0xA1, 0x01);

	/* masquage des interruptions */
	outb(0x21, 0x00);
	outb(0xA1, 0x00);

	/*asm("mov $0x20, %al \n \
		outb %al, $0x20");

	asm("mov $0x20, %al \n \
	outb %al, $0xA0 \n \
	outb %al, $0x20");*/
}

void resetPIC()
{
	//outb(0xA1, 0xFF);
	//outb(0x21, 0xFF);
	outb(0x20, 0x11);
	outb(0xA0, 0x11);

	outb(0x21, 0x08);
	outb(0xA1, 0x70);

	outb(0x21, 0x04);
	outb(0xA1, 0x01); // !!! C'est 1 et non 2  , http://www.rohitab.com/discuss/topic/35103-switch-between-real-mode-and-protected-mode/

	outb(0x21, 0x01);
	outb(0xA1, 0x01);
}

