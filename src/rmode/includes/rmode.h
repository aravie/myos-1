
#ifndef _RMODE_H
#define _RMODE_H

extern "C" void rmode_int(int int_num);
void int10x86(X86Regs in, X86Regs *out);
void int13x86(X86Regs in, X86Regs *out);
//void int13x86(X86Regs in, X86Regs *out);

#endif

