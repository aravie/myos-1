

#include "macros.h"
#include "device.h"



Device::Device(int bus_num, int device_num)
{
	this->bus_num = bus_num;
	this->device_num = device_num;

	//cli;
	unsigned long address = 0x80000000 | ((bus_num & 0xFF) << 16) | ((device_num & 0x1F) << 11); // On va lire le vendeur
	//outl(PCI_CONFIG_ADDRESS, address);
	// Registre 0 pour aller chercher les ID du periph
	unsigned long reg0, reg2;
	reg0 = inl(PCI_CONFIG_DATA);
	//sti;
	vendorID = reg0 & 0xFFFF;
	deviceID = reg0 >> 16;

	address += 8;
	outl(PCI_CONFIG_ADDRESS, address);
	reg2 = inl(PCI_CONFIG_DATA);

	classcode = (0xFF000000 & reg2) >> 24;
	subclass = (0xFF0000 & reg2) >> 16;
}

void Device::writeRegister(int register_offset, long data)
{
    unsigned long address = 0x80000000 | (bus_num << 16) | (device_num << 11) | (register_offset & 0xFC);
    outl(PCI_CONFIG_ADDRESS, address);
    outl(PCI_CONFIG_DATA, data);
}

int Device::readRegister(int register_offset)
{
    unsigned long address = 0x80000000 | (bus_num << 16) | (device_num << 11) | (register_offset & 0xFC);
    outl(PCI_CONFIG_ADDRESS, address);
    return inl(PCI_CONFIG_ADDRESS);
}


/*int pciCheckVendor(unsigned short bus, unsigned short slot)
 {
    int vendor,device;
    vendor = 0;
    device = 0;
    vendor = pciConfigReadWord(bus,slot,0,0);
    return vendor;
 }*/
