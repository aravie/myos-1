

#include "types.h"
#include "includes/rmode.h"
#include "../debug/includes/debug.h"
#include "macros.h"
#include "idt_manager.h"

extern IDTPtr idtptr;

// On a créé une pile rmode_stack dans l'espace du mode réel pour pouvoir l'utiliser quand on revient dans ce mode
void int10x86(X86Regs in, X86Regs *out)
{
	u16 *top_stack = (u16*)0x6FFE; // La pile reel sera placé à cet endroit
	*top_stack = in.ax;
	top_stack--;
	*top_stack = in.bx;
	top_stack--;
	*top_stack = in.cx;
	top_stack--;
	*top_stack = in.dx;
	top_stack--;
	*top_stack = in.es;
	top_stack--;
	*top_stack = in.di;
	top_stack--;
	*top_stack = in.si;
	//top_stack--;
	//*top_stack = in.ds;

	// Remarque sur PUSH : (sur la machine de dvpt) POP, esp pointe sur la valeur qu'il va lire PUIS il incrémente.

	resetPIC();
	rmode_int(0x10);
	configurePIC();

	out->si = *top_stack;
	top_stack++;
	out->di = *top_stack;
	top_stack++;
	out->es = *top_stack;
	top_stack++;
	out->dx = *top_stack;
	top_stack++;
	out->cx = *top_stack;
	top_stack++;
	out->bx = *top_stack;
	top_stack++;
	out->ax = *top_stack;
}

void int13x86(X86Regs in, X86Regs *out)
{
	u16 *top_stack = (u16*)0x6FFE; // La pile reel sera placé à cet endroit
	*top_stack = in.ax;
	top_stack--;
	*top_stack = in.bx;
	top_stack--;
	*top_stack = in.cx;
	top_stack--;
	*top_stack = in.dx;
	top_stack--;
	*top_stack = in.es;
	top_stack--;
	*top_stack = in.di;
	top_stack--;
	*top_stack = in.si;
	//top_stack--;

	// Remarque sur PUSH : (sur la machine de dvpt) POP, esp pointe sur la valeur qu'il va lire PUIS il incrémente.

	resetPIC();
	rmode_int(0x13);
	configurePIC();

	//out->ds = *top_stack;
	//top_stack++;
	out->si = *top_stack;
	top_stack++;
	out->di = *top_stack;
	top_stack++;
	out->es = *top_stack;
	top_stack++;
	out->dx = *top_stack;
	top_stack++;
	out->cx = *top_stack;
	top_stack++;
	out->bx = *top_stack;
	top_stack++;
	out->ax = *top_stack;
}



