

#include "types.h"

#include "ata_device.h"
#include "macros.h"

#include "../debug/includes/debug.h"


ATADevice::ATADevice(int drive)
{
	this->drive = drive;
}


void ATADevice::init(int block, int sector_count)
{
	//outb(0x1F7, 0xE7);
	outb(0x1F0, 0);
	outb(0x1F2, sector_count);
	outb(0x1F3, (unsigned char) block);
	outb(0x1F4, (unsigned char) (block >> 8));
	outb(0x1F5, (unsigned char) (block >> 16));
	outb(0x1F6, 0xE0 | (drive << 4) | ((block >> 24) & 0x0F));
}

void ATADevice::read(int block, int sector_count, char *buffer)
{
	u16 tmp;
	int idx;

	init(block, sector_count);
	outb(0x1F7, 0x20);

	while(!(inb(0x1F7) & 0x08));
 // On attend de recevoir le signal qui dit que le periph est prêt

	//if(inb(0x1F7) & 0x8)
	//	debug("il y a eu une erreur");

	for (idx = 0; idx < 256 * sector_count; idx++) {
		tmp = inw(0x1F0);
		buffer[idx * 2] = (unsigned char) tmp;
		buffer[idx * 2 + 1] = (unsigned char) (tmp >> 8);
	}
}

void ATADevice::write(int block, int sector_count, const char *buffer)
{
	u16 tmp;
	int idx;

	init(block, sector_count);
	outb(0x1F7, 0x30);

	// on attend le signal du periph (voir read)
	while (!(inb(0x1F7) & 0x08));

	for (idx = 0; idx < 256 * sector_count; idx++) {
			tmp = (buffer[idx * 2 + 1] << 8) | buffer[idx * 2];
			outw(0x1F0, tmp);
	}
}


