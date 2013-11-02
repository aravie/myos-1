

#ifndef _TASKS_MANAGER_H
#define _TASKS_MANAGER_H

#include "types.h"
#include "singleton.h"
#include "task.h"
#include "linked_list.h"

using namespace MyLibrary;





struct TSSDescriptor {
	    u16    previous_task, __previous_task_unused;
	    u32    esp0;
	    u16    ss0, __ss0_unused;
	    u32    esp1;
	    u16    ss1, __ss1_unused;
	    u32    esp2;
	    u16    ss2, __ss2_unused;
	    u32    cr3;
	    u32    eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
	    u16    es, __es_unused;
	    u16    cs, __cs_unused;
	    u16    ss, __ss_unused;
	    u16    ds, __ds_unused;
	    u16    fs, __fs_unused;
	    u16    gs, __gs_unused;
	    u16    ldt_selector, __ldt_sel_unused;
	    u16    debug_flag, io_map;
	} __attribute__ ((packed));



class TasksManager : public Singleton<TasksManager>
{
	friend class Singleton<TasksManager>;
	friend class Task;
public:
	// Il faut initialiser dans le noyau pour avoir les bonnes valeurs de la pile (esp, ebp)
	void init();

	void createTask(void *taskAddr);
	//void launchTask(Task *task);

	void startScheduling();

	// Cette fonction est le trigger qui sera appelé tous les tops d'horloge
	// Elle se chargera de gérer l'ordonnancement des tâches
	void trigger();

	void exec() { listOfTasks.last()->exec(); }

	Task *getNextTask();

	void exitCurrentTask();
	void switchToTask(int n);


	TSSDescriptor *getKernelTSS() const { return &kernel_tss; }

private:
	static TSSDescriptor kernel_tss;
	MyLibrary::LinkedList<Task*> listOfTasks;
	Task *currentTask;
	int next_ID;
	bool scheduling;
};

#endif

