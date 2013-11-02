
#include "isr.h"
#include "debug.h"
#include "macros.h"
#include "vga.h"
#include "vesa.h"
#include "keyboard_event.h"
#include "event_manager.h"
#include "system_calls_manager.h"
#include "tasks_manager.h"
#include "drawer.h"
#include "string.h"


#define KEYBOARD_BUFFER_SIZE 256


char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
unsigned char bcursor = 0; // Curseur sur le buffer


extern "C" void page_fault()
{
	debug("Page fault\n");
	int cr2;
	asm("mov %%cr2, %%eax \n \
		mov %%eax, %0" :"=m"(cr2):);
	Screen->print("Error on : "); Screen->putHex((u32)cr2);

	dumpRegisters();

	debug("\n");
	debug(TasksManager::getInstance()->getKernelTSS()->esp0);

	drawer->print(String::fromUtf8("Page fault"));
	drawer->print(String::fromNumber((u32)cr2, 16));
	drawer->drawPixel(0,0, 0xff, 0x0, 0);

	while(1);
}

extern "C" void general_protection_fault()
{
	//drawer->drawPixel(0,0xFF, 0x0, 0x0, 0);
	debug("General protection fault\n");
	//drawer->drawPixel(400,200, 0x0, 0xff, 0);
	while(1);
}



extern "C" void irq_0() // Clock
{
	static short i = 0;
	/*static bool b = false;
	if(!b)
	{
		b = true;
	}*/
	//TasksManager::getInstance()->trigger();

	if (i < 4) {
		i++;
	} else {
		i = 0;
		//dumpRegisters();
		TasksManager::getInstance()->trigger();
	}
}


// Keyboard
extern "C" void irq_1()
{
	// On attend que le scancode soit écrit dans le buffer du PIC
	unsigned int i=0;
	do {
	    i = inb(0x64);
	} while((i & 0x01) == 0);

	unsigned char c = inb(0x60); // on récupère le caractère
	c--;

	//debug("p");
	KeyboardEvent::KeyState k = (c>=0x80) ? KeyboardEvent::RELEASED : KeyboardEvent::PRESSED;
	c = (c>=0x80) ? c - 0x80: c;
	//Kernel::heap._printHeapState();
	KeyboardEvent *v = new KeyboardEvent(k, c);	//Vesa::getInstance()->putPixel(400,200, 0x0, 0xff, 0);
	//Kernel::heap._printHeapState();
	EventManager::getInstance()->pushEvent(v);
	//keybdoard_buffer[bcursor] = c;
	//Screen->putChar(c);
	//bcursor++;
	//dumpRegisters();
}



extern "C" void system_calls(int syscall_num, int param)
{
	//Screen->print("Sys");
	SystemCallsManager::getInstance()->receiveSystemCall(syscall_num, param);
}


extern "C" void irq_12()
{
	//Screen->print("Utilisation du bus principal\n");
}



extern "C" void irq_14()
{
	//Screen->print("Utilisation du bus principal\n");
}

extern "C" void irq_15()
{
	//Screen->print("Utilisation du bus secondaire\n");
}



extern "C" void default_interrupt()
{

	debug("default interrupt\n");
	//drawer->drawPixel(0xFF,0x0, 0x0, 0x0, 0);
	//drawer->drawPixel(400,200, 0xff, 0xff, 0xff);
	//while(1);
}

