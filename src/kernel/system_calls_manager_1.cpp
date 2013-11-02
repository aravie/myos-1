
#include "types.h"

#include "system_calls_manager.h"
#include "tasks_manager.h"
#include "debug.h"
#include "vga.h"
#include "drawer.h"
#include "string.h"

void SystemCallsManager::receiveSystemCall(int callID, int param)
{
	switch(callID)
	{
	case 1: // sys_exit
		TasksManager::getInstance()->exitCurrentTask();
	break;
	case 2:
		debug("ID=2");
	break;
	case 3:
	{
		char *addr2 = (char*)param;
		Screen->print(addr2);
	}
	break;
	case 4:
	{
		char *msg = (char *)param;
		String str = msg;
		drawer->print(str);
	}
	break;
	case 5:
	break;
	case 6:
	break;
	}
}

