
#include "debug.h"
#include "../graphics/includes/vga.h"


void debug(const char* msg)
{
	Screen->print(msg);
}

void debug(unsigned char msg)
{
	Screen->putHex(msg);
}

void debug(u32 nb)
{
	Screen->putHex(nb);
	Screen->putChar('\n');
}

void printAddr(void *ptr)
{
	Screen->print("0x");
	for(int i=7; i>=0; i--)
	{
		char pack = (char)((u32)ptr >> i*4) & 0xF;
		pack += (pack<10)?'0': 'A'-10;
		Screen->putChar(pack);
	}
}

extern "C" void dumpRegisters()
{
	u32 eax, ebx, ecx, edx, cr0, cr3, esp, ebp;
	u16 cs, ds, es, fs, gs, ss, flags;

	asm("movl %%eax, %0 \n \
		movl %%ebx, %1 \n \
		movl %%ecx, %2 \n \
		movl %%edx, %3 \n \
		movw %%cs, %4 \n \
		movw %%ds, %5 \n \
		movw %%es, %6 \n \
		movw %%fs, %7 \n \
		movw %%gs, %8 \n \
		movw %%ss, %9 \n \
		movl %%esp, %10 \n \
		movl %%ebp, %11 \n \
		movl %%cr0, %%eax \n \
		movl %%eax, %12 \n \
		movl %%cr3, %%eax \n \
		movl %%eax, %13 \n \
		pushfw \n \
		popw %14 \n":
		"=m"(eax), "=m"(ebx),"=m"(ecx), "=m"(edx),
		 "=m"(cs), "=m"(ds), "=m"(es), "=m"(fs), "=m"(gs), "=m"(ss),
		 "=m"(esp), "=m"(ebp), "=m"(cr0), "=m"(cr3), "=m"(flags):);


	Screen->print("\nEAX=");
	Screen->putHex(eax);
	Screen->print("/EBX=");
	Screen->putHex(ebx);
	Screen->print("/ECX=");
	Screen->putHex(ecx);
	Screen->print("/EDX=");
	Screen->putHex(edx); Screen->print("\n");
	Screen->print("CS=");
	Screen->putHex(cs);
	Screen->print("/DS=");
	Screen->putHex(ds);
	Screen->print("/ES=");
	Screen->putHex(es);
	Screen->print("/GS=");
	Screen->putHex(gs);
	Screen->print("/FS=");
	Screen->putHex(fs);
	Screen->print("/SS=");
	Screen->putHex(ss);
	Screen->print("\n");
	Screen->print("ESP=");
	Screen->putHex(esp);
	Screen->print("/EBP=");
	Screen->putHex(ebp);
	Screen->print("/CR0=");
	Screen->putHex(cr0);
	Screen->print("/CR3=");
	Screen->putHex(cr3);
	Screen->print("/FLAGS=");
	Screen->putHex(flags);
}
