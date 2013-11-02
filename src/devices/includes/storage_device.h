

#ifndef _STORAGE_H
#define _STORAGE_H

#include "../../file/includes/file_system.h"


#define STORAGE_BUFFER 0x3000
#define SECTOR_SIZE 512

class StorageDevice
{
public:
	StorageDevice();

	int (*createFile)(const char *filename);
	void (*removeFile)(int id);

	int getHeads() const { return heads; }
	int heads, cylinders, sectors_per_track, tracks;
	int bytes_per_sector;

private:
	void readSector(int lba, char *buffer);
	void writeSector(int lba, char *buffer);


};

#endif
