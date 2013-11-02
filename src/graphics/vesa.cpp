

#include "types.h"
#include "vesa.h"
#include "../rmode/includes/rmode.h"
#include "utils.h"
#include "vga.h"
#include "debug.h"
#include "fonts/myfont"


VbeInfoBlock Vesa::vbeInfoBlock;
PixelCursor Vesa::pixel_cursor;

Vesa::Vesa()
{
	vbeInfoBlock.VbeSignature[0] = 0x0;
	vbeInfoBlock.VbeSignature[1] = 0x0;
	vbeInfoBlock.VbeSignature[2] = 0x0;
	vbeInfoBlock.VbeSignature[3] = 0x0;
	pixel_cursor.x =0;
	pixel_cursor.y =0;
	XResolution = 0;
	YResolution = 0;
	bytesPerPixel = 0;
	subwinNumber = 0;
	winSize = 0x0;
	currentBank = 0;

	queryVbeInfoBlock();
}

u16 Vesa::queryVbeInfoBlock()
{
	//debug("GET VBE INFO BLOCK\n");
	X86Regs reg, out;
	reg.ax = 0x4F00;
	reg.di = (u32)(&vbeInfoBlock) & 0xFFFF;
	reg.es = ((u32)(&vbeInfoBlock) & 0xFFFF0000) >> 4;
	int10x86(reg, &out);


	return out.ax; // retour de l'appel de la fonction {voir VBE doc}
}

u16 Vesa::getModeInfoBlock(u16 mode, ModeInfoBlock *block)
{
	X86Regs reg, out;
	reg.ax = 0x4F01;
	reg.bx = 0;
	reg.cx = mode;
	reg.dx = 0;
	reg.di = (u16)(((u32)(block)) & 0xFFFF);
	reg.es = (u16)(((u32)(block) & 0xFFFF0000) >> 4);
	int10x86(reg, &out);

	return out.ax;
}


void Vesa::printModeInformation(u16 mode)
{
	ModeInfoBlock block;
	block.XResolution = 0;
	block.YResolution = 0;
	getModeInfoBlock(mode, &block);

	Screen->print("Informations sur le mode ");
	Screen->putHex(mode);
	Screen->print("\nXResolution=");
	Screen->putNb(block.XResolution);
	Screen->print("\nYResolution=");
	Screen->putNb(block.YResolution);
	Screen->print("\nBits per pixel=");
	Screen->putNb(block.bitsPerPixel);
	Screen->print("\nBuffer addr=");
	Screen->putHex(block.physBasePtr[0]);
	Screen->print("\nOff Screen memory addr=");
	Screen->putHex(block.offScreenMemOffset[0]);
	Screen->print("\nBytes per scanline=");
	Screen->putNb(block.bytesPerScanline);
	Screen->print("\nBankSize=");
	Screen->putNb(block.bankSize);
	Screen->print("\nNumber of Banks=");
	Screen->putHex(block.numberOfBanks);
	Screen->print("\nWinASegment=");
	Screen->putHex(block.winASegment);
	Screen->print("\nWinBSegment=");
	Screen->putHex(block.winBSegment);
	Screen->print("\nMemoryModel=");
	Screen->putHex(block.memoryModel);
	Screen->print("\nWinGranularity=");
	Screen->putHex(block.winGranularity);
	Screen->print("\nWinSize=");
	Screen->putHex(block.winSize);
	Screen->print("\nWinFuncPtr=");
	Screen->putHex(block.winFuncPtr);
	Screen->print("\nModeAttributes=");
	Screen->putHex(block.modeAttributes);

	Screen->print("\n");
}

u16 Vesa::setMode(u16 mode)
{
	ModeInfoBlock currentModeInfo;
	X86Regs reg, out;
	reg.ax = 0x4F02;
	reg.bx = mode;
	reg.cx = 0;
	reg.dx = 0;
	reg.es = 0;
	reg.di = 0;
	int10x86(reg, &out);

	this->getModeInfoBlock(mode, &currentModeInfo);
	XResolution = (int)currentModeInfo.XResolution & 0xFFFF;
	YResolution = (int)currentModeInfo.YResolution & 0xFFFF;
	bytesPerPixel = (currentModeInfo.bitsPerPixel+1) / 8; // On calcule la ditance qu'on doit parcourir en terme d'octets pour atteindre le pixel adjacent
	winSize = currentModeInfo.winGranularity << 10;
	subwinNumber = (XResolution * YResolution * bytesPerPixel) / winSize;
	currentBank = 0;
	charWidth = 9;
	charHeight = 9;
	charPerLine = XResolution / charWidth;


	return out.ax;
}


void Vesa::getVesaVersion(char *version_str)
{
	u16 version = vbeInfoBlock.VbeVersion;
	u8 major = (version & 0xFF00) >> 8;
	u8 minor = version & 0xFF;

	char maj[11], min[11];
	convert_nb_to_string(major, maj);
	convert_nb_to_string(minor, min);

	version_str[0] = maj[0];
	version_str[1] = '.';
	version_str[2] = min[0];
	version_str[3] = 0;	
}

void Vesa::printModeList()
{
	u16 segment = vbeInfoBlock.VideoModePtr[1];
	u16 offset = vbeInfoBlock.VideoModePtr[0];
	u16 *list = (u16*)(segment << 4 | offset);
	for(int i=1; *list != 0xFFFF; i++)
	{
		Screen->putNb(i);
		Screen->print(" : ");
		Screen->putHex(*list);
		Screen->putChar('\n');
		list++;
	}
}

// Retourne la taille de la mémoire graphique en kOctets
u32 Vesa::getMemorySize()
{
	u32 size = vbeInfoBlock.TotalMemory;
	size <<= 6;
	return size;
}

u16 Vesa::switchToBank(int n)
{
	X86Regs reg, out;
	reg.ax = 0x4F05;
	reg.bx = 0;
	reg.cx = 0;
	reg.dx = n;
	reg.es = 0;
	reg.di = 0;
	int10x86(reg, &out);

	currentBank = n;

	return out.ax;
}


void Vesa::putPixel(u16 x, u16 y, u8 red, u8 green, u8 blue)
{
	if(x >= XResolution || y >= YResolution || x < 0 || y < 0)
		return;

	int offset = (y * XResolution + x)*bytesPerPixel;

	int bank = 0;

	if(winSize != 0) bank = offset / winSize;
	offset %= winSize;

	if(bank != currentBank)
		Vesa::switchToBank(bank);

	char *video=(char*)(RAMVIDEO_ADDRESS + offset);

	u16 attr = (red & 0x1F) << 11 | (green & 0x3F) << 5 | (blue & 0x1F);
	video[0] = attr & 0xFF;
	video[1] = (attr & 0xFF00) >> 8;
}


// Routine de base d'affichage d'un pixel par cette méthode (VESA)
void Vesa::drawPixel(u16 x, u16 y, u8 red, u8 green, u8 blue)
{
	Vesa::getInstance()->putPixel(x,y,red,green,blue);
}


