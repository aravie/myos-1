
#ifndef _SYSTEM_CALLS_MANAGER_H
#define _SYSTEM_CALLS_MANAGER_H

#include "singleton.h"


#define SYS_EXIT 1
#define DEBUG 2
#define PRINT_VGA 3
#define PRINT_VESA 4
#define FILE_READ 5
#define FILE_WRITE 6


class SystemCallsManager : public Singleton<SystemCallsManager>
{
	friend class Singleton<SystemCallsManager>;

public:
	void receiveSystemCall(int callID, int param);


private:

};



#endif
