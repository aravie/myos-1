

#ifndef _VGA_H
#define _VGA_H

#include "singleton.h"

#define RVIDEO_ADDRESS (unsigned char*)0xB8000
#define RVIDEO_SIZE (unsigned int)0xFA0

#define INIT_BG_COLOR	Vesa::BLACK
#define INIT_FG_COLOR	Vesa::WHITE


#include "types.h"

struct CursorPosition
{
	unsigned char row;
	unsigned char col;
};


class Vga : public Singleton<Vga>
{
	friend class Singleton<Vga>;

	public:
	enum Color {
		BLACK 		= 	0x00,
		BLUE 		= 	0x01,
		GREEN 		= 	0x02,
		CYAN 		= 	0x03,
		RED 		= 	0x04,
		MAGENTA 	= 	0x05,
		BROWN 		= 	0x06,
		LIGHT_GRAY 	= 	0x07,
		DARK_GRAY 	= 	0x08,
		LIGHT_BLUE 	= 	0x09,
		LIGHT_GREEN 	= 	0x0A,
		LIGHT_CYAN 	= 	0x0B,
		LIGHT_RED 	= 	0x0C,
		LIGHT_MAGENTA 	= 	0x0D,
		YELLOW 		= 	0x0E,
		WHITE 		= 	0x0F
	};


	void cleanScreen(); // clean the screen with the background color, erase all chars
	void setBackgroundColor(Color bg_color); // set the background color, must clean screen to apply
	Vga::Color getBackgroundColor() { return background_color; }

	void setForegroundColor(Color fg_color); // set the foreground color
	Vga::Color getForegroundColor() { return foreground_color; }

	void scrollup(const int n);

 	void putChar(unsigned char c, Color fg_color, Color bg_color); // Put a char on screen at current cursor position
	void print(const char *str, Color fg_color, Color bg_color); // Put a string on screen
	void putHex(u8 c, Color fg_color, Color bg_color); 
	void putNb(u32 c, Color fg_color, Color bg_color); 

	void putChar(unsigned char c); // Put a char on screen at current cursor position
	void print(const char *str); // Put a string on screen
	void putHex(u8 c); 
	void putNb(u32 nb); 

	void putHex(u32 nb);
	void putHex(u16 nb);

	void putChar(unsigned char c, Color fg_color); // Put a char on screen at current cursor position
	void print(const char *str, Color fg_color); // Put a string on screen

	void printAddress(void *ptr); // Affiche une adresse à l'écran

	private:
	Vga(); // On bloque l'instanciation pour le singleton

	Color background_color, foreground_color;
	CursorPosition cursor;

};

#define Screen Singleton<Vga>::getInstance()
//#define screen  *((Vga*)Singleton<Vga>::getInstance())


#endif // _VESA_H


