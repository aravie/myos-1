

#ifndef USB_DEVICE_H
#define USB_DEVICE_H

#include "pci_device.h"

class UsbDevice : public PCIDevice
{
public:
	UsbDevice(const PCIDevice& dev);
	UsbDevice(int bus_num, int device_num);

private:

};

#endif
