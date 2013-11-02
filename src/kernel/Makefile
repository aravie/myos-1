

OBJ_DIR=....releaseobj

INCLUDES=	-Iincludes 
 					-I..includes 
 					-I..libsincludes 
 					-I..interruptsincludes 
 					-I..graphicsincludes 
 					-I..memoryincludes 
 					-I..mylibraryincludes 
 					-I..devicesincludes 
 					-I..debugincludes 
 					-I..rmodeincludes 


CPP_SRC=kernel.cpp 
				gdt_manager.cpp 
				idt_manager.cpp 
				test.cpp 
				event_manager.cpp 
				main.cpp 
				tasks_manager.cpp 
				system_calls_manager.cpp 
				task.cpp 
				process.cpp

CPP_OBJ_FILE=$(CPP_SRC.cpp=.o)

CPP_OBJ=$(addprefix $(OBJ_DIR), $(CPP_OBJ_FILE))


all $(CPP_OBJ)

$(OBJ_DIR)%.o %.cpp
	$(CPP_CC) $(INCLUDES) $ -o $(OBJ_DIR)$@
	

clean 
	rm .o

