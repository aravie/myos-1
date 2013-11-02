

#include "event_listener.h"
#include "event.h"
#include "queue.h"
#include "debug.h"
#include "stdlib.h"
#include "keyboard_event.h"

EventListener::EventListener()
{
	trigger = true;
	active = true;

}

void EventListener::pushEvent(Event *e)
{
	queueEvent.push(e);
}


void EventListener::flush()
{
	while(!queueEvent.isEmpty())
	{
		delete queueEvent.pop();
	}
}


Event* EventListener::next()
{
	if(queueEvent.count() == 0)
	{
		return NULL;
	}
	return queueEvent.pop();
}
