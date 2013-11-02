
#include "types.h"
#include "vga.h"
#include "utils.h"



Vga::Vga()
{
	foreground_color = WHITE;
	cursor.row = 0;
	cursor.col = 0;
	//cleanScreen();
}

// clean the screen with the background color, erase all chars
void Vga::cleanScreen()
{
	unsigned char *video = RVIDEO_ADDRESS;

	// On parcourt la mémoire vidéo, on la vide et on écrit les bits de couleurs de fond dans les attributs
	for(unsigned int i=0; i<RVIDEO_SIZE; i++)
		video[i]=0;

	setBackgroundColor(Vga::BLACK);
	setForegroundColor(Vga::WHITE);
	cursor.row = 0;
	cursor.col = 0;
}

void Vga::setBackgroundColor(Color bg_color)
{
	background_color = bg_color;
	unsigned char *video = RVIDEO_ADDRESS;
	for(unsigned int i=1; i<RVIDEO_SIZE; i+=2)
		video[i] = (background_color << 4) & 0xF0;
}

void Vga::setForegroundColor(Color fg_color)
{
	foreground_color = fg_color;
	unsigned char *video = RVIDEO_ADDRESS;
	for(unsigned int i=1; i<RVIDEO_SIZE; i+=2)
			video[i] = (foreground_color) & 0x0F;
}

void Vga::scrollup(const int n)
{
	unsigned char *video = RVIDEO_ADDRESS;
	const unsigned int pitch = 2*80;
	const unsigned int npitch = n*pitch;

	for(unsigned int i=0; i<RVIDEO_SIZE; i++) // On va copier 25 lignes - 1, celle du haut
	{
		video[i] = video[i+npitch];
	}

	for(unsigned int i=RVIDEO_SIZE-npitch; i<RVIDEO_SIZE; i+=2)
	{
		video[i]=0;
		video[i+1]= ((background_color << 4) & 0xF0) | (foreground_color & 0x0F);
	}
}


void Vga::putChar(unsigned char c, Color fg_color, Color bg_color)
{
	unsigned char *video = RVIDEO_ADDRESS;
	video += (cursor.row * 80 + cursor.col) * 2;

	if(c == '\n') // Si on a affaire à un retour chariot
	{
		cursor.row++;
		cursor.col = 0;
	}
	else if(c == '\t') // Une tabulation
		cursor.col += 4;
	else
	{
		*video = c;
		video++;
		*(video) = bg_color << 4 | fg_color;
		cursor.col++;
	}

	if(cursor.col >= 80)
	{
		cursor.col = 0;
		cursor.row++;
	}

	if(cursor.row>24)
	{
		scrollup(1);
		cursor.col=0;
		cursor.row--;
	}
}

void Vga::putHex(u8 c, Color fg_color, Color bg_color)
{
	u8 digit1, digit2;
	digit1 = (c & 0xF0) >> 4;
	digit2 = c & 0xF;	
	digit1 += (digit1 >= 10) ?'A'-10: '0';
	digit2 += (digit2 >= 10) ?'A'-10: '0';
	print("0x", fg_color, bg_color);	
	putChar(digit1);
	putChar(digit2);
}

void Vga::putHex(u32 nb)
{
	print("0x");
	for(int i=7; i>=0; i--)
	{
		char pack = (char)(nb >> i*4) & 0xF;
		pack += (pack<10)?'0': 'A'-10;
		putChar(pack);
	}
}

void Vga::putHex(u16 nb)
{
	print("0x");
	for(int i=3; i>=0; i--)
	{
		char pack = (char)(nb >> i*4) & 0xF;
		pack += (pack<10)?'0': 'A'-10;
		putChar(pack);
	}
}


void Vga::putHex(u8 c)
{
	putHex(c, foreground_color, background_color);
}



void Vga::putNb(u32 nb, Color fg_color, Color bg_color)
{
	char tab[11]; 
	convert_nb_to_string(nb, tab);
	print(tab, fg_color, bg_color);
}

void Vga::putNb(u32 nb)
{
	putNb(nb, foreground_color, background_color);
}


void Vga::putChar(unsigned char c)
{
	putChar(c, foreground_color, background_color);
}

void Vga::putChar(unsigned char c, Color fg_color)
{
	putChar(c, fg_color, background_color);
}


void Vga::print(const char *str, Color fg_color, Color bg_color)
{
	char c = str[0];
	int i=0;

	while(c != '\0')
	{
		putChar(c, fg_color, bg_color);
		i++;
		c = str[i];
	}
}

void Vga::print(const char *str)
{
	print(str, foreground_color, background_color);
}

void Vga::print(const char *str, Color fg_color)
{
	print(str, fg_color, background_color);
}

void Vga::printAddress(void *ptr)
{
	print("0x");
	for(int i=7; i>=0; i--)
	{
		char pack = (char)((u32)ptr >> i*4) & 0xF;
		pack += (pack<10)?'0': 'A'-10;
		putChar(pack);
	}
}



