
#include "stdlib.h"

void strcpy(char *src, char *dest, int len)
{
	for(int i=0; i<len; i++)
	{
		dest[i] = src[i];
	}
}
