

#ifndef _DEVICE_H
#define _DEVICE_H

class Device
{
public:
	int getVendorID() const { return vendorID; }
	int getDeviceID() const  { return deviceID; }
	int getSubClass() const { return subclass; }
	int getClassCode() const { return classcode; }

protected:
	int vendorID, deviceID;
	int subclass, classcode;

};

#endif
