
#ifndef _KEYBOARD_LISTENER_H
#define _KEYBOARD_LISTENER_H

#include "event_listener.h"
#include "keyboard_event.h"
#include "types.h"

class KeyboardListener : public EventListener
{
	friend class KeyboardEvent;
public:
	KeyboardListener();

	virtual void pushEvent(Event *e);

	KeyboardEvent *next();

	void enableBuffer(uchar *p, unsigned int b_s);
	void disableBuffer();

	bool isBufferized() const { return pbuffer != 0; };

	u8 getBufferCursor() const { return kl_cursor; }


private:
	unsigned int buffer_size;
	u8 kl_cursor;
	uchar *pbuffer;
};

#endif

