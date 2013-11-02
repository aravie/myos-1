
#ifndef _DEVICES_MANAGER_H
#define _DEVICES_MANAGER_H

#include "singleton.h"
#include "linked_list.h"
#include "device.h"


#define BUS_NUMBER_MAX 256
#define DEVICE_NUMBER_MAX 32


class DevicesManager : public Singleton<DevicesManager>
{
	friend class Singleton<DevicesManager>;
public:
	void scanDevices();

	int deviceCount() const { return listOfDevices.size(); }

	// Renvoie le periph en position pos dans la liste
	Device *device(int pos);

private:
	DevicesManager();

	MyLibrary::LinkedList<Device*> listOfDevices;
};

#endif
