

#ifndef _PCI_DEVICE_H
#define _PCI_DEVICE_H

#include "device.h"

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC


class PCIDevice : public Device
{
public:
	PCIDevice(int bus_num, int device_num);

	void writeRegister(int register_offset, long data);
	int readRegister(int register_offset);

private:
	int bus_num, device_num;
};

#endif
