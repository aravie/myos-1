
#include "event_manager.h"
#include "linked_list.h"
#include "queue.h"
#include "event.h"
#include "event_listener.h"
#include "keyboard_event.h"
#include "../graphics/includes/vga.h"
#include "../includes/macros.h"
#include "debug.h"


EventManager::EventManager()
{
	active = true;
}

void EventManager::pushEvent(Event *e)
{
	//debug("push");
	queueEvent.push(e);
	dispatch();
}


void EventManager::dispatch()
{
	if(!active)
		return; // Si le manager n'est pas activé, on inhibe les évènements

	if(listEventListener.count() > 0 && queueEvent.count() > 0)
	{
		while(queueEvent.count() != 0)
		{
			Event *e = queueEvent.pop();
			if(e != 0)
			{
				for(int i=0; i<listEventListener.count(); i++)
				{
					switch(e->getType())
					{
					case Event::KEYBOARD_EVENT:
						KeyboardEvent *k = new KeyboardEvent(*(static_cast<KeyboardEvent*>(e)));
						listEventListener.at(i)->pushEvent(k);
						break;
					}
				}
				delete e;
			}
		}
	}
}

void EventManager::installListener(EventListener *listener)
{
	listEventListener.pushLast(listener);
}

/*void EventManager::getKeyboardBuffer(char *buffer, u8 *cursor)
{
	u8 cpy_cursor = *cursor;
	for(; kcursor!=cpy_cursor; kcursor++)
	{
		u8 c = buffer[kcursor];
		KeyboardEvent::KeyState st;
		if(c > 0x80)
		{
			c -= 0x80;
			st = KeyboardEvent::RELEASED;
		}
		else
		{
			st = KeyboardEvent::PRESSED;
		}
		KeyboardEvent *e = new KeyboardEvent(st, c);
		queueEvent.push(e);
	}

	dispatch();
}*/
