
#include "types.h"
#include "utils.h"
#include "vga.h"


void convert_nb_to_string(u32 nb, char* tab)
{
	for(int i=0; i<=10; tab[i++]=0); // Elle initialise le tableau
	u8 digit;

	if(nb == 0)
	{
		tab[0] = '0';
		tab[1] = 0;
		return;
	}

	for(int i=9; i>=0 && nb!=0; i--)
	{
		digit = nb % 10;
		nb -= digit;
		nb /= 10;
		tab[i] = digit + '0';
	}

	int j; // j va contenir la position du premier caractère utile dans la chaine
	for(j=0; tab[j] == 0 && j<10; j++);

	for(int i=0;j<=10; j++, i++)
	{
		tab[i] = tab[j];
	}	
}


void memcpy(char *src, char *dest, unsigned int size)
{
	for(unsigned int i=0; i<size; i++)
		dest[i]=src[i];
}

/*extern "C" void * memmove(void * dst, const void * src, unsigned int len)
{
        memcpy((char*)src, (char*)dst, len);
       return dst;
}*/


void dump(unsigned char *src, unsigned int size)
{
	for(unsigned int i=0; i<size; i++)
	{
		Screen->putHex(*src++);
		Screen->putChar('-');
	}
}
