

#include "usb_device.h"


UsbDevice::UsbDevice(const PCIDevice& dev)
: PCIDevice(dev)
{


}

UsbDevice::UsbDevice(int bus_num, int device_num)
: PCIDevice(bus_num, device_num)
{


}
