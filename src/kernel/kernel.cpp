

#include "kernel.h"

#include "types.h"
#include "macros.h"

#include "utils.h"
#include "rmode.h"
#include "vga.h"
#include "vesa.h"
#include "heap.h"
#include "devices_manager.h"
#include "storage_device.h"
#include "usb_device.h"
#include "tasks_manager.h"

#include "keyboard_listener.h"
#include "event_manager.h"

#include "task.h"
#include "process.h"

//#include "keymap_azerty.h"

#include "string.h"
#include "drawer.h"

void testGraphic();


using namespace MyLibrary;

Heap Kernel::heap;



Kernel::Kernel()
{
	cli;
	Screen->print("Creation du tas propre au kernel .... ");
	heap.initHeap(Heap::KERNEL, KERNEL_HEAP_BOTTOM, KERNEL_HEAP_TOP);
	Screen->print("OK\n", Vga::LIGHT_GREEN);

	Screen->print("Test des interruptions en mode reel .... ");
	X86Regs reg, out;
	reg.ax = 0x0200;
	reg.bx = 0x0000;
	int10x86(reg, &out); // Remet le curseur en position 0,0 grâce au bios
	Screen->print("OK\n", Vga::LIGHT_GREEN);

	Screen->print("Implementation des singletons .... ");
	Screen->print("OK\n", Vga::LIGHT_GREEN);

	Screen->print("Test Allocation dynamique .... ");
	Screen->print("OK\n", Vga::LIGHT_GREEN);

	Screen->print("Test de VESA VBE en mode reel .... ");
	Screen->print("OK\n", Vga::LIGHT_GREEN);

	Screen->print("-----------------------\n");
	Screen->print("Informations sur VESA\n", Vga::CYAN);
	//char version[5];
	//Vesa::getInstance()->getVesaVersion(version);
	Screen->print("Version de VESA : ");
	//Screen->print(version);
	Screen->print("\nMemoire graphique : ");
	//Screen->putNb(Vesa::getInstance()->getMemorySize());
	Screen->print(" kB\n");
	Screen->print("List of compatible modes :\n");
	//Vesa::getInstance()->printModeList();
	Screen->print("-----------------------\n");

	//Vesa::getInstance()->printModeInformation(0x011A);
	Screen->print("Test d'ecriture sur un disque IDE .... ");
	Screen->print("OK\n", Vga::LIGHT_GREEN);
	Screen->print("Test de lecture sur un disque IDE .... ");
	Screen->print("OK\n", Vga::LIGHT_GREEN);

	DevicesManager::getInstance()->scanDevices();
	int device_count = DevicesManager::getInstance()->deviceCount();
	Screen->print(String::fromNumber(device_count).data());
	Screen->print("\n");
	UsbDevice *usbHost = 0;

	for(int i=0; i<device_count; i++)
	{
		Device *d = DevicesManager::getInstance()->device(i);
		Screen->print(String::fromNumber(d->getVendorID(), 16).data());
		Screen->print(" ");
		Screen->print(String::fromNumber(d->getDeviceID(), 16).data());
		Screen->print("\n");
		if(d->getDeviceID() == 0x24CD)
			usbHost = (UsbDevice*)d;
	}



}


void Kernel::exec()
{
	/*Vesa::getInstance()->setMode(0x011A);
	const int xres = Vesa::getInstance()->getXResolution();
	const int yres = Vesa::getInstance()->getYResolution();
	drawer->setScreenResolution(xres, yres);
	drawer->setDrawPixelPrimitive(Vesa::drawPixel);

	String bienvenue("Bonjour et BIENVENUE SUR MYOS\ncree par Clement MICHAUD\n\n\n");
	drawer->print(bienvenue);
	String str = "Ceci est un test de String";

	for(int p = 0; p<5; p++)
		drawer->print(bienvenue);


	DevicesManager::getInstance()->scanDevices();
	int device_count = DevicesManager::getInstance()->deviceCount();

	String pci_devices = "Vous avez actuellement " + String::fromNumber(device_count) + " peripheriques sur le bus PCI\n";
	drawer->print(pci_devices);
*/

	/*if(sortie == 0x004F)
	{
		drawer->print(String::fromUtf8("Bon"));
	}else
	{
		drawer->print(String::fromNumber(sortie, 16));
	}*/

	//TasksManager::getInstance()->init();
	//heap._printHeapState();
	//TasksManager::getInstance()->createTask((void*)&task3);
	//TasksManager::getInstance()->createTask((void*)&task2);
	//TasksManager::getInstance()->createTask((void*)&task1);
	//TasksManager::getInstance()->startScheduling();
//	testGraphic();

	sti;

	while(1);
	//loop();
}



void Kernel::loop()
{
	KeyboardListener *keyboardListener = new KeyboardListener;
	EventManager::getInstance()->installListener(keyboardListener);

	KeyboardEvent *kbe = 0;
	char c;
	int i=0;

	while(1)
	{

		kbe = keyboardListener->next();
		//Screen->putChar('c');

		if(kbe != 0)
		{
			if(kbe->getKeyState() == KeyboardEvent::PRESSED)
			{
				c = kbe->getLetter();
				//Screen->putChar(c);
				//for(int j=0; j<8; j++)
				//drawer->drawPixel(i,0, 255,255,255);
				drawer->putChar(c);
				i++;
			}
		}

		if(kbe != 0)
		{
			delete kbe;
			kbe = 0;
		}
	}
}




