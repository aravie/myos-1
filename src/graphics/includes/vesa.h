
#ifndef _VESA_H
#define _VESA_H

#include "singleton.h"
#include "cursor.h"

#define RAMVIDEO_ADDRESS (u8*)0xA0000

// Attention C'est une structure compatible 16 bits, pas de 32 bits
// Bloc de 512 octets
struct VbeInfoBlock
{
	u8 VbeSignature[4];
	u16 VbeVersion;
	u16 OemStringPtr[2];
	u8 Capabilities[4];
	u16 VideoModePtr[2];
	u16 TotalMemory;
	u16 OemSoftwareRev;
	u16 OemVendorNamePtr[2];
	u16 OemProductNamePtr[2];
	u16 OemProductRevPtr[2];
	u8 Reserved[222];
	u8 OemData[256];
} __attribute__ ((packed));


// Bloc de 256 octets qui décrit un mode donné
struct ModeInfoBlock
{
	u16 modeAttributes;
	u8 winAAttributes;
	u8 winBAttributes;
	u16 winGranularity;
	u16 winSize;
	u16 winASegment;
	u16 winBSegment;
	u32 winFuncPtr;
	u16 bytesPerScanline;
	u16 XResolution;
	u16 YResolution;
	u8 XCharSize;
	u8 YCharSize;
	u8 numberOfPlanes;
	u8 bitsPerPixel;
	u8 numberOfBanks;
	u8 memoryModel;
	u8 bankSize; // en koctets
	u8 numberOfImagesPages;
	u8 reserved;
	u8 redMaskSize;
	u8 redFieldPosition;
	u8 greenMaskSize;
	u8 greenFieldPosition;
	u8 blueMaskSize;
	u8 blueFieldPosition;
	u8 rsvdMaskSize;   // Voir la doc de VBE
	u8 rsvdFieldPosition;
	u8 directColorModeInfo;
	u16 physBasePtr[2];
	u16 offScreenMemOffset[2];
	u16 offScreenMemSize;
	u8 reserved2[206];

}__attribute__ ((packed));


struct CRTCInfoBlock
{
	u16 horizontalTotal;
	u16 horizontalSyncStart;
	u16 horizontalSyncEnd;
	u16 verticalTotal;
	u16 verticalSyncStart;
	u16 verticalSyncEnd;
	u8 flags;
	u16 pixelClock[2];
	u16 refreshRate;
	u8 reserved[40];
}__attribute__((packed));


struct PixelCursor
{
	u16 x, y;
};



class Vesa : public Singleton<Vesa>
{
	friend class Singleton<Vesa>;

	public:
	u16 queryVbeInfoBlock();
	VbeInfoBlock getVbeInfoBlock() { return vbeInfoBlock; }
	void printModeList();
	u16 getModeInfoBlock(u16 mode, ModeInfoBlock *block);
	void printModeInformation(u16 mode);

	int getXResolution() const  { return XResolution; }
	int getYResolution() const { return YResolution; }

	// On définit le mode 'mode', et on retourne l'état de ax en sortie de la fonction VESA
	u16 setMode(u16 mode);

	void getVesaVersion(char *);
	u32 getMemorySize();


	static void drawPixel(u16 x, u16 y, u8 red, u8 green, u8 blue);

	void putPixel(u16 x, u16 y, u8 red, u8 green, u8 blue);

	private:
	Vesa();
	u16 switchToBank(int n);

	// Informations générales sur vesa, chargé à la création d'une instance de Vesa
	static VbeInfoBlock vbeInfoBlock;
	static PixelCursor pixel_cursor;
	int XResolution;
	int YResolution;
	u8 bytesPerPixel;
	u8 subwinNumber;
	u8 currentBank;
	u8 charWidth, charHeight, charPerLine;
	u32 winSize; // en octets
};


#endif

