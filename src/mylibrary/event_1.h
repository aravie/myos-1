

#ifndef _EVENT_H
#define _EVENT_H

#include "types.h"

class Event
{
public:
	enum EventType{ KEYBOARD_EVENT };
	enum Priority { LOW, NORMAL, HIGH };

	Event(EventType t);
	EventType getType() { return type; };

protected:

	Priority priority;
	EventType type;
};

#endif
