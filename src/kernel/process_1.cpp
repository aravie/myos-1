
#include "process.h"
#include "keyboard_listener.h"
#include "event_manager.h"
#include "string.h"


extern char keyboard_buffer[256];
extern u8 bcursor;




void task1()
{
	char *msg = (char *) 0x40001000;
	int i,j=0;


		msg[0] = 't';
		msg[1] = 'a';
		msg[2] = 's';
		msg[3] = 'k';
		msg[4] = '1';
		msg[5] = ' ';
		msg[6] = '0';
		msg[7] = '\n';
		msg[8] = 0;

		while (1) {
			msg[6] = '0' + j;
			asm("mov %0, %%ebx; mov $0x04, %%eax; int $0x30":: "m"(msg));
			j++;
			for (i = 0; i < 5000000; i++);
			//j++;
			if(j==10)
				asm("mov $0x01, %eax; int $0x30");
		}
}

void task2()
{
	int i=0;
	char *msg = (char*)0x40001000;
				msg[0] = 'T';
				msg[1] = 'A';
				msg[2] = 'S';
				msg[3] = 'K';
				msg[4] = '2';
				msg[5] = '\0';
				asm("mov $0x03, %%eax \n \
							mov %0, %%ebx \n \
							int $0x30" :: "m"(msg));

				asm("mov $0x01, %eax \n int $0x30");

	while(1)
	{
	}
}



void task3()
{
	//MyLibrary::String *str = (MyLibrary::String *)0x40002000;
	//str->String();
	char *msg = (char *) 0x40001000;


	msg[0] = 't';
	msg[1] = 'a';
	msg[2] = 's';
	msg[3] = 'k';
	msg[4] = '3';
	msg[5] = ' ';
	msg[6] = '\n';
	msg[7] = 0;
	msg[8] = 0;
	//str->operator=(msg);
	//*str = msg;

	//asm("mov %0, %%ebx; mov $0x02, %%eax; int $0x30":: "m"(str));
		while (1) {
			for (int i = 0; i < 5000000; i++);
			asm("mov %0, %%ebx; mov $0x04, %%eax; int $0x30":: "m"(msg));
		}
}

