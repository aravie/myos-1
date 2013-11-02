
#include "types.h"

#include "kernel.h"
#include "vga.h"
#include "gdt_manager.h"
#include "idt_manager.h"
#include "my_memory_manager.h"



extern "C" void __cxa_pure_virtual()
{
}

int main();

struct Entry
{
	u32 memorySize;
	u16 bootDrive;
} __attribute__((packed));


extern "C" void _cpp_kernel()
{
	Screen->putChar('C');
	Screen->cleanScreen();
	Screen->print("********* Bienvenue sur MyOS *********\n\n", Vga::YELLOW);

	Screen->print("Test d'affichage d'un texte avec VGA .... ");
	Screen->print("OK\n", Vga::LIGHT_GREEN);

	Screen->print("Taille de la memoire .... ");
	//Screen->putNb(entry.memorySize);
	Screen->print(" kB\n");

	Screen->print("Peripherique de boot .... ");
	//Screen->putNb(entry.bootDrive);
	Screen->print("\n");


	Screen->print("Installation de la nouvelle GDT .... ");
	GDTManager::getInstance()->initGDT();
	Screen->print("OK\n", Vga::LIGHT_GREEN);

	Screen->print("Installation de la nouvelle IDT .... ");
	IDTManager::getInstance()->initIDT();
	Screen->print("OK\n", Vga::LIGHT_GREEN);

	Screen->print("Configuration du PIC .... ");
	configurePIC();
	Screen->print("OK\n", Vga::LIGHT_GREEN);

	Screen->print("Initialisation de la pagination .... ");
	MyMemoryManager::getInstance()->initMemoryManagement();
	Screen->print("OK\n", Vga::LIGHT_GREEN);


	// On met la pile en 0x20000 puis on crée une nouvelle stack frame en rentrant dans main
	__asm__("mov $0x18, %ax \n \
			mov %ax, %ss \n \
			movl $0x20000, %esp \n \
			movl $0x20000, %ebp \n \
			jmp _next \n \
			_next:");

	main();
}


int main()
{
	Kernel kernel;

	// boucle infinie
	kernel.exec();
	return 0;
}
