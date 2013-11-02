
#include "devices_manager.h"
#include "pci_device.h"
#include "usb_device.h"

#include "macros.h"
#include "debug.h"


DevicesManager::DevicesManager()
{
}

void DevicesManager::scanDevices()
{
	int bus_num, device_num;
	unsigned long address;
	int vendor;
	bool device_on_bus = true;
	PCIDevice *device;

	for(bus_num=0; bus_num< BUS_NUMBER_MAX && device_on_bus; bus_num++)
	{
		device_on_bus = false;
		for(device_num=0;device_num < DEVICE_NUMBER_MAX; device_num++)
		{
			address = 0x80000000 | ((bus_num & 0xFF) << 16) | ((device_num & 0x1F) << 11); // On va lire le vendeur
			outl(PCI_CONFIG_ADDRESS, address);
			vendor = inl(PCI_CONFIG_DATA) & 0xFFFF;
			if(vendor != 0xFFFF)
			{
				device_on_bus = true;
				device = new PCIDevice(bus_num, device_num);
				if(device->getVendorID() == 0x8086 && device->getDeviceID() == 0x24CD)
				{
					delete device;
					UsbDevice *usb_device = new UsbDevice(*device);
					listOfDevices.pushLast(usb_device);
				}else
				{
					listOfDevices.pushLast(device);
				}
			}
		}
	}
}


Device *DevicesManager::device(int pos)
{
	return listOfDevices.at(pos);
}

