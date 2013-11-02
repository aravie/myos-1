

INCLUDES=	-I../includes \
					-I../libs/includes \
					-Iincludes \
					-I../debug/includes \
					-I../libs/includes \
					-I../kernel/includes \
					-I../memory/includes \
					-I../graphics/includes

OBJ_DIR=../../release/obj


CPP_SRC=	event_listener.cpp \
					keyboard_event.cpp \
					keyboard_listener.cpp \
					event.cpp \
					color.cpp \
					string.cpp \
					stdlib.cpp

CPP_OBJ_FILE=$(CPP_SRC:.cpp=.o)

CPP_OBJ=$(addprefix $(OBJ_DIR)/, $(CPP_OBJ_FILE))


all: $(CPP_OBJ)

$(OBJ_DIR)/%.o: %.cpp
	$(CPP_CC) $(INCLUDES) $< -o $(OBJ_DIR)/$@
	

clean: 
	rm *.o