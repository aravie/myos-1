
#include "task.h"
#include "my_memory_manager.h"
#include "utils.h"
#include "tasks_manager.h"



Task::Task(const int TID)
{
	this->TID = TID;
}

Task::Task(const int TID, void *taskAddr)
{
	this->TID = TID;
	this->taskAddr = taskAddr;

	context.es = 0x3B;
	context.fs = 0x3B;
	context.gs = 0x3B;

	/*context.eax = 0;
	context.ebx = 0;
	context.ecx = 0;
	context.edx = 0;
	context.edi = 0;
	context.esi = 0;*/

	context.cs = 0x33;
	context.ds = 0x3B;
	context.ss = 0x43;
	context.esp = (u32)0x40001000;
	//context.ebp = (u32)0x40001000;
	context.eip = (u32)0x40000000;

	// Crée un répertoire de page de la taille voulue
	context.cr3 = (u32)MyMemoryManager::getInstance()->createTaskEnvironment(0x3000);
	launched = false;

	load();
}

void Task::load()
{
	MyMemoryManager::getInstance()->loadTaskInMemory(*this);
}

void Task::exec()
{
	u32 eip = context.eip;
	u32 esp=context.esp;
	u32 eflags = context.eflags;
	u32 esi = context.esi;
	u32 edi = context.edi;
	u32 ebp = context.ebp;

	u16 ds = context.ds;
	u16 cs = context.cs;
	u16 ss = context.ss;
	u16 gs = context.gs;
	u16 fs = context.fs;
	u16 es = context.es;

	u32 eax, ebx, ecx, edx;
	eax = context.eax;
	ebx = context.ebx;
	ecx = context.ecx;
	edx = context.edx;

	// Charge le contexte de la tache (la table de repertoires)
	//heap.initHeap(Heap::USER, (void*)(0x40002000), (void*)0x40006000);

	// On peut passer des paramètres sur la pile utilisateur
	//userStack[0] = 0x40002000;
	//userStack--;
	MyMemoryManager::getInstance()->loadTaskPageDirectory(*this);
	eflags = (eflags | 0x200) & 0xFFFFBFFF;

	u32 esp0 = TasksManager::getInstance()->getKernelTSS()->esp0 ;
	asm("movl %0, %%esp" :: "m"(esp0));

	if(TID != 0)
	{
		asm("   push %0 \n \
			push %1 \n"
			:: "m"(ss), "m"(esp) );
	}

	asm("	pushl %0 \n"
			"pushl %1 \n"
			"pushl %2 \n"
			"pushl %3"::
				"m"(eflags), "m"(cs), "m"(eip), "m"(ebp)
			);

	asm __volatile__(" mov $0x20, %al \n outb %al, $0x20 \n cli");
	// On a stoppé les interruptions pour éviter qu'on n'interrompt un basculement de tâche, on risque de perdre le fil


	asm(		"pushl %0 \n "
				"pushl %1 \n "
				"pushl %2 \n "
				"pushl %3 \n "
				"pushl %4 \n "
				"pushl %5 \n"
				 ::"m"(eax), "m"(ebx), "m"(ecx), "m"(edx), "m"(esi), "m"(edi)
				);

		asm("pushl %0 \n"
				"pushl %1 \n"
				"pushl %2 \n"
				"pushl %3 \n "
				:: "m"(ds),"m"(es),"m"(fs),"m"(gs));

	asm("popl %gs \n"
			"popl %fs \n"
			"popl %es \n"
			"popl %ds \n"
			"popl %edi \n"
			"popl %esi \n"
			"popl %edx \n"
			"popl %ecx \n"
			"popl %ebx \n"
			"popl %eax \n"
			"popl %ebp \n"
	);

	asm("sti \n iret");
}



void Task::saveState(const TaskContext& c)
{
	context.ss = c.ss;
	context.cs = c.cs;
	context.eflags = c.eflags;
	context.ds = c.ds;
	context.eip = c.eip;
	context.esp = c.esp;

	context.esi = c.esi;
	context.ebp = c.ebp;
	context.edi = c.edi;
	context.gs = c.gs;
	context.es = c.es;
	context.fs = c.fs;
	context.eax = c.eax;
	context.ebx = c.ebx;
	context.ecx = c.ecx;
	context.edx = c.edx;
}


