
#ifndef _DEBUG_H
#define _DEBUG_H

#include "types.h"

void debug(const char* msg);
void debug(unsigned char msg);
void debug(u32 nb);

extern "C" void dumpRegisters();

void dump(unsigned char *addr, unsigned int nb);

void printAddr(void *ptr);

#endif
