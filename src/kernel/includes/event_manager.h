
#ifndef _EVENT_MANAGER_H
#define _EVENT_MANAGER_H

#include "singleton.h"
#include "linked_list.h"
#include "queue.h"

class Event;
class EventListener;


class EventManager : public Singleton<EventManager>
{
	friend class Singleton<EventManager>;
public:

	void pushEvent(Event *e);
	void dispatch();

	void installListener(EventListener *listener);

	// Transforme en evenement les touches appuyées sur le clavier (flush le buffer et actualise le cursor du buffer)
	//void getKeyboardBuffer(char *buffer, u8 *cursor);

	int count() { return queueEvent.count();}

private:
	EventManager();

	// Permet d'inhiber les évènements si on le souhaite
	bool active;
	MyLibrary::Queue<Event *> queueEvent;
	MyLibrary::LinkedList<EventListener *> listEventListener;

};


#endif



