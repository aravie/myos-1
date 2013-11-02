

#ifndef _ATADEVICE_H
#define _ATADEVICE_H


#include "device.h"

class ATADevice : public Device
{
public:
	ATADevice(int drive);

	void init(int block, int sector_count);

	void read(int block, int sector_count, char *buffer);
	void write(int block, int sector_count, const char *buffer);

	//void format();



private:
	int drive;
};

#endif
 
