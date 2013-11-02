
#include "event.h"
#include "queue.h"

Event::Event(EventType t)
{
	priority = NORMAL;
	type = t;
}
