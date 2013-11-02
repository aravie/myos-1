
#ifndef _UTILS_H
#define _UTILS_H

void convert_nb_to_string(u32 nb, char* tab);

void memcpy(char *src, char *dest, unsigned int size);
//extern "C" void* memmove(void * dst, const void * src, unsigned int len);
void dump(unsigned char *src, unsigned int size);


#endif

