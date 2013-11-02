

OBJ_DIR=../../release/obj

INCLUDES=	-Iincludes \
 					-I../includes \
 					-I../libs/includes \
 					-I../interrupts/includes \
 					-I../graphics/includes \
 					-I../memory/includes \
 					-I../mylibrary/includes \
 					-I../devices/includes \
 					-I../debug/includes \
 					-I../rmode/includes


CPP_SRC=kernel.cpp \
				gdt_manager.cpp \
				idt_manager.cpp \
				test.cpp \
				event_manager.cpp \
				main.cpp \
				tasks_manager.cpp \
				system_calls_manager.cpp \
				task.cpp \
				process.cpp


CPP_OBJ_FILE=$(CPP_SRC.cpp=.o)

CPP_OBJ=$(addprefix $(OBJ_DIR), $(CPP_OBJ_FILE))


all: $(CPP_OBJ)

$(OBJ_DIR):%.o %.cpp
	$(CPP_CC) $(INCLUDES) $ -o $(OBJ_DIR)$@
	

clean:
	rm .o

