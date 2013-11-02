
#ifndef _ISR_H
#define _ISR_H


#define come_in_ISR asm __volatile__ ("pushal")

#define out_of_ISR asm __volatile__("	movb $0x20, %al \n  \
				outb %al, $0x20 \n \
				popal \n \
				leave \n \
				iret")

#define out_of_ISR2 asm __volatile__("	movb $0x20, %al \n  \
				outb %al, $0x20 \n \
				leave \n \
				iret")
// Instruction leave super importante, elle recharge la bonne valeur de esp (évite le plantage)


extern "C" void page_fault();
extern "C" void general_protection_fault();
extern "C" void irq_0();
extern "C" void irq_1();
extern "C" void system_calls(int syscall_num, int param); // System calls
extern "C" void irq_12();
extern "C" void irq_14();
extern "C" void irq_15();
extern "C" void default_interrupt();


// Les points d'entrées dans le fichier asm
extern "C" void _page_fault();
extern "C" void _general_protection_fault();
extern "C" void _irq_0();
extern "C" void _irq_1();
extern "C" void _system_calls(); // System calls
extern "C" void _irq_12();
extern "C" void _irq_14();
extern "C" void _irq_15();
extern "C" void _default_interrupt();

#endif


