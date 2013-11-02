

#ifndef _MACROS_H
#define _MACROS_H

#define outb(port, data) __asm__(" outb %%al, %%dx" :: "d"(port), "a"(data) );

#define inb(port) ({ \
	unsigned char __c; \
	__asm__("inb %%dx, %%al" : "=a"(__c) : "d"(port)); \
	__c; \
	})

#define outw(port, data) __asm__(" outw %%ax, %%dx" :: "d"(port), "a"(data) );

#define inw(port) ({ \
	unsigned short __c; \
	__asm__("inw %%dx, %%ax" : "=a"(__c) : "d"(port)); \
	__c; \
	})

#define inl(port) ({    \
	unsigned long _v;       \
	asm volatile ("inl %%dx, %%eax" : "=a" (_v) : "d" (port)); \
        _v;     \
})

#define outl(port, data) \
	asm volatile ("outl %%eax, %%dx" :: "d" (port), "a" (data))

#define sti __asm__("sti; nop; nop"::)
#define cli __asm__("cli"::)



#define pushad int esp, ebp, eax, ebx, ecx, edx, esi, edi; \
				short ds, es, fs, gs; \
			asm __volatile__(" \
				mov %%eax, %0 \n \
				mov %%ebx, %1 \n \
				mov %%ecx, %2 \n \
				mov %%edx, %3 \n \
				mov %%esi, %4 \n \
				mov %%edi, %5 \n \
				mov %%ds,  %6 \n \
				mov %%es, %7 \n \
				mov %%fs, %8 \n \
				mov %%gs, %9 \n \
				mov %%esp, %10 \n \
				mov %%ebp, %11 \n \
				push %%eax \n \
				movw $0x10, %%ax \n \
				movw %%ax, %%ds \n \
				pop %%eax \n \
				 \n" : "=m"(eax),"=m"(ebx),"=m"(ecx),"=m"(edx),"=m"(esi),"=m"(edi), \
				"=m"(ds),"=m"(es),"=m"(fs),"=m"(gs),"=m"(esp), "=m"(ebp):  )


#define popad asm __volatile__(" \
				mov %0, %%eax \n \
				mov %1, %%ebx \n \
				mov %2, %%ecx \n \
				mov %3, %%edx \n \
				mov %4, %%esi \n \
				mov %5, %%edi \n \
				mov %6, %%ax \n \
				mov %%ax, %%ds \n \
				mov %7, %%ax \n \
				mov %%ax, %%es \n \
				mov %8, %%ax \n \
				mov %%ax, %%fs \n \
				mov %9, %%ax \n \
				mov %%ax, %%gs \n \
				mov %10, %%esp \n \
				mov %11, %%ebp \n \
				" :: "m"(eax),"m"(ebx),"m"(ecx),"m"(edx),"m"(esi),"m"(edi), \
				"m"(ds),"m"(es),"m"(fs),"m"(gs),"m"(esp), "m"(ebp):  )





#define pushad1	asm __volatile__(" \
				mov $0xFFFFFFFF, %edx \n \
				push $0xFFFFFFFF \n \
				push %ebx \n \
				push %ecx \n \
				push %edx \n \
				push %esi \n \
				push %edi \n \
				push %ds \n \
				push %es \n \
				push %fs \n \
				push %gs \n \
				push %ax \n \
				movw $0x10, %ax \n \
				movw %ax, %ds \n \
				pop %ax \n \
				 \n" )


#define popad1 asm __volatile__(" \
				pop %gs \n \
				pop %fs \n \
				push %es \n \
				push %ds \n \
				push %edi \n \
				push %esi \n \
				push %edx \n \
				push %ecx \n \
				push %ebx \n \
				push %eax \n \
				 \n"  )




/*
#define pushad asm __volatile__("\
				pushl %eax \n \
				mov $0x10, %ax \n \
				mov %ax, %ds \n \
	")


#define popad asm __volatile__("\
					popl %edi \n \
					popl %esi \n \
					popl %ebp \n \
					popl %esp \n \
					popl %edx \n \
					popl %ecx \n \
					popl %ebx \n \
					popl %eax")
*/
#endif // _MACROS_H


