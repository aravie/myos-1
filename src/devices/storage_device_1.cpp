
#include "types.h"
#include "storage_device.h"
#include "rmode.h"
#include "my_file_system.h"
#include "stdlib.h"


StorageDevice::StorageDevice()
{
	createFile = (*MyFileSystem::createFile); //(const char*);
	removeFile = (*MyFileSystem::removeFile);

	X86Regs reg, out;
	reg.ax = 0x0800;
	reg.bx = 0x0000;
	reg.cx = 0x0000;
	reg.dx = 0x0080;
	reg.es = 0;
	reg.di = 0;
	reg.si = 0;
	int13x86(reg, &out);

	/*u16 *buff = (u16*)0x3000;
	cylinders = *(buff+2);
	heads = *(buff);
	sectors_per_track = *(buff+6);
	bytes_per_sector = *(buff+18);*///*(buff+12) & 0xFFFF;
	//heads = out.cx;
	//heads = 456;

	char *buffer = new char[512];
	readSector(0, buffer);
	delete buffer;
}


void StorageDevice::readSector(int lba, char *buffer)
{
	X86Regs reg, out;

	char *storage_buf = (char*)STORAGE_BUFFER;

	//int cylinder, head, track, sector;
	reg.ax = 0x0201;
	reg.cx = 0x0001;
	reg.dx = 0x0080;
	reg.bx = STORAGE_BUFFER;
	reg.es = 0x0000;
	int13x86(reg, &out);

	for(int i=0;i<512; i++)
			buffer[i] = storage_buf[i];
}

void StorageDevice::writeSector(int lba, char *buffer)
{
	X86Regs reg, out;

	char *storage_buf = (char*)STORAGE_BUFFER;
	for(int i=0;i<512; i++)
		storage_buf[i] = buffer[i];

	//int cylinder, head, track, sector;
	reg.ax = 0x0301;
	reg.cx = 0x0001;
	reg.dx = 0x0080;
	reg.bx = STORAGE_BUFFER;
	reg.es = 0x0000;
	int13x86(reg, &out);
}

