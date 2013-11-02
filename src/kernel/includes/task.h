
#ifndef _TASK_H
#define _TASK_H

#include "memory_manager.h"
#include "heap.h"

#define SIZE 1000

#define USER_TASK_ADDRESS 	0x40000000
#define MAX_HEAP  		  	0xFFFFFFFF


struct TaskContext {
			u32 eax, ecx, edx, ebx;
			u32 esp, ebp, esi, edi;
			u32 eip, eflags;
			u16 cs, ss, ds, es, fs, gs;
			u32 cr3;
	} __attribute__ ((packed));



class Task
{
	friend class MemoryManager;
	friend class TasksManager;

public:
	int getTaskID() const { return TID; }

private:
	Task(const int TID);
	Task(const int TID, void *taskAddr);

	// Charge en mémoire la tâche
	void load();

	void exec();

	void saveState(const TaskContext& c);
	const TaskContext& getContext() { return context; }

	Heap heap;
	TaskContext context;
	int TID; // Task ID
	void *taskAddr;
	bool launched;
};

#endif
