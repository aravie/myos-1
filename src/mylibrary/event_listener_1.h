
#ifndef _EVENT_LISTENER
#define _EVENT_LISTENER

#include "event.h"
#include "queue.h"

class EventListener
{
public:
	EventListener();

	virtual void pushEvent(Event *e);

	void flush();
	virtual Event* next();

protected:
	MyLibrary::Queue<Event *> queueEvent;
	bool trigger;
	bool active;
};

#endif
