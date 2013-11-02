

#include "keyboard_listener.h"
#include "keyboard_event.h"
#include "../graphics/includes/vga.h"
#include "debug.h"

extern uchar keymap_azerty[];



KeyboardListener::KeyboardListener()
{
	kl_cursor = 0;
	buffer_size = 0;
	pbuffer = 0;
}


void KeyboardListener::pushEvent(Event *e)
{
	//debug("passage");
	KeyboardEvent *kbe = static_cast<KeyboardEvent*>(e);

	if(pbuffer != 0 && kl_cursor<buffer_size-1 && kbe->getKeyState() == KeyboardEvent::PRESSED)
	{
		pbuffer[kl_cursor] = kbe->getKeyCode();
		pbuffer[kl_cursor + 1] = '\0';
		kl_cursor++;
	}
	EventListener::pushEvent(e);
}


KeyboardEvent *KeyboardListener::next()
{
	//debug("test");
	if(queueEvent.count() == 0)
		return 0;


	KeyboardEvent *e;
	if(queueEvent.size() > 0)
	{
		//debug("test");
		e = (KeyboardEvent*)queueEvent.pop();
	}
	else
		e=0;

	return e;
}


void KeyboardListener::enableBuffer(uchar *p, unsigned int b_s)
{
	pbuffer = p;
	buffer_size = b_s;
	kl_cursor = 0;
}


void KeyboardListener::disableBuffer()
{
	pbuffer = 0;
	kl_cursor = 0;
}

