
#include "tasks_manager.h"
#include "my_memory_manager.h"
#include "gdt_manager.h"
#include "../debug/includes/debug.h"
#include "task.h"
#include "linked_list.h"

TSSDescriptor TasksManager::kernel_tss;



void TasksManager::init()
{
	scheduling = false;
	currentTask = 0;
	TasksManager::kernel_tss.debug_flag = 0x00;
	TasksManager::kernel_tss.io_map = 0x00;
	TasksManager::kernel_tss.esp0 = 0x20000;
	TasksManager::kernel_tss.ss0 = 0x18;
	TasksManager::kernel_tss.ds = 0x10;

	asm("mov %%esp, %0" :"=m"(kernel_tss.esp0):);

	GDTManager *gdtm = GDTManager::getInstance();
	GDTDescriptor *gdt = (GDTDescriptor *)GDT_BASE;
	// On réserve un descripteur pour le TSS du noyau
	gdtm->createDescriptor((u32) &TasksManager::kernel_tss, 0x67, 0xE9, 0x00, &gdt[9]); // TSS du noyau 0x48

	// On charge le registre Task Register (TR)
	asm("	movw $0x48, %ax \n \
				ltr %ax");

	Task *kernel_thread = new Task(0);
	kernel_thread->context.cr3 = (u32)MyMemoryManager::getInstance()->kernelPageDirectory;
	kernel_thread->context.cs = 0x8;
	kernel_thread->context.ds = 0x10;
	currentTask = kernel_thread;
	listOfTasks.pushLast(kernel_thread);
	next_ID = 1;
}

void TasksManager::createTask(void *taskAddr)
{
	Task *t = new Task(next_ID, taskAddr);
	listOfTasks.pushLast(t);
	next_ID++;
}

void TasksManager::startScheduling()
{
	scheduling = true;
}

void TasksManager::trigger()
{
	u32 *stackPtr;

	if(!scheduling)
			return;

	// On récupère un pointeur sur la position actuelle de la pile
	// On appelle trigger après une interruption donc les registres de la tâche sont sur la pile.
	asm("mov (%%ebp), %%eax \n \
			mov %%eax, %0"
			: "=m"(stackPtr):);


	if(listOfTasks.count() == 0)
		return;
	/*else if(listOfTasks.count() == 1)
	{
		Task *t = getNextTask();
		//if(currentTask != t)
		{
			currentTask = t;
			t->exec();
		}
	}*/
	else // Si on a plus d'1 tache
	{
		//int i=
		TaskContext ct;

		ct.eflags = stackPtr[16];
		ct.cs  = stackPtr[15];
		ct.eip = stackPtr[14];
		ct.eax = stackPtr[13];
		ct.ecx = stackPtr[12];
		ct.edx = stackPtr[11];
		ct.ebx = stackPtr[10];
		ct.esp = stackPtr[9];
		ct.ebp = stackPtr[8];
		ct.esi = stackPtr[7];
		ct.edi = stackPtr[6];
		ct.ds = stackPtr[5];
		ct.es = stackPtr[4];
		ct.fs = stackPtr[3];
		ct.gs = stackPtr[2];

		int offset_esp;
		if(currentTask->context.cs != 0x8)
		{
			ct.esp = stackPtr[17];
			ct.ss = stackPtr[18];
			offset_esp = 19;
		}
		else
			offset_esp = 17;

		currentTask->saveState(ct);


		kernel_tss.esp0 = (u32) (stackPtr + offset_esp); // On vide la stack frame

		Task *t = getNextTask();
		currentTask = t;
		currentTask->exec();
	}
}



Task *TasksManager::getNextTask()
{
	static int i=0;
	if(i >= listOfTasks.count())
		i = 0;
	Task *t = listOfTasks.at(i);
	i++;
	return t;
}

// Exit the current task and switch to another one
void TasksManager::exitCurrentTask()
{
	listOfTasks.deleteFirstItem(currentTask);
	delete currentTask;
	currentTask = getNextTask();
	currentTask->exec();
}


void TasksManager::switchToTask(int n)
{

}



