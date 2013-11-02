
#ifndef _KEYBOARD_EVENT_H
#define _KEYBOARD_EVENT_H

#include "types.h"
#include "event.h"



class KeyboardEvent : public Event
{
	friend class KeyboardListener;
public:
	enum KeyState { PRESSED, RELEASED };

	struct KeyEvent
	{
		KeyState keyState;
		u8 code;
	};

	KeyboardEvent(KeyState state, u8 pcode);


	const KeyState getKeyState() const { return keyEvent.keyState; }
	const u8 getKeyCode() const { return keyEvent.code; }

	u8 getLetter() const;

private:
	KeyEvent keyEvent;
};



#endif

