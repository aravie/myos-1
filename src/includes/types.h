
#ifndef _TYPES_H
#define _TYPES_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char uchar;

struct X86Regs
{
	u16 ax, bx, cx, dx;
	u16 es, di, si;
};


#endif // _TYPES_H


