
#include "gdt_manager.h"
#include "utils.h"
#include "../debug/includes/debug.h"
#include "tasks_manager.h"


GDTPtr gdtptr;

void GDTManager::initGDT()
{
	GDTDescriptor gdt[GDT_SIZE];
	createDescriptor(0x0, 0x0, 0x0, 0x0, &gdt[0]);		// Descripteur nul
	createDescriptor(0x00, 0xFFFFF, 0x9B, 0x0D, &gdt[1]);  // CS   0x8
	createDescriptor(0x00, 0xFFFFF, 0x93, 0x0D, &gdt[2]);	// DS 0x10
	createDescriptor(0x00, 0x0, 0x97, 0x0D, &gdt[3]); // Pile  0x18
	
	// On crée des descripteurs pour le mode protégé 16 bits qui serviront à passer en mode reel
	createDescriptor(0x0, 0xFFFF, 0x9A, 0x00, &gdt[4]);  // CS 16-bits  0x20
	createDescriptor(0x0, 0xFFFF, 0x92, 0x00, &gdt[5]);  // DS 16-bits  0x28

	createDescriptor(0x00, 0xFFFFF, 0xFF, 0x0D, &gdt[6]); // Code utilisateur 0x30
	createDescriptor(0x00, 0xFFFFF, 0xF3, 0x0D, &gdt[7]); // Data utilisateur 0x38
	createDescriptor(0x0, 0x0,    0xF7, 0x0D, &gdt[8]); // Pile utilisateur 0x40


	gdtptr.base = GDT_BASE;
	gdtptr.limit = GDT_SIZE*GDT_ENTRY_SIZE;


	memcpy((char*)gdt, (char*)gdtptr.base, gdtptr.limit);

	__asm__("	lgdtl (gdtptr) \n \
						movw $0x10, %ax \n \
						movw %ax, %ds \n \
						movw %ax, %es \n \
						movw %ax, %fs \n \
						movw %ax, %gs \n \
						ljmp $0x08, $_flush \n \
						_flush: \n"
				);
}


void GDTManager::createDescriptor(u32 base, u32 limit, u8 access, u8 flags, GDTDescriptor *desc)
{
	desc->base0_15 = base & 0xFFFF;
	desc->base16_23 = (base & 0xFF0000) >> 16;
	desc->base24_31 = (base & 0xFF000000) >> 24;
	desc->limit0_15 = limit & 0xFFFF;
	desc->limit16_19 = (limit & 0xF0000) >> 16;
	desc->access_byte = access;
	desc->flags = (flags & 0xF); 
}

