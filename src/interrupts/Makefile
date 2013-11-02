

OBJ_DIR=../../release/obj

INCLUDES=		-Iincludes \
						-I../graphics/includes \
						-I../includes \
						-I../debug/includes \
						-I../mylibrary/includes \
						-I../kernel/includes \
						-I../libs/includes \
						-I../memory/includes


CPP_SRC=isr.cpp

CPP_OBJ_FILE=$(CPP_SRC:.cpp=.o)

CPP_OBJ=$(addprefix $(OBJ_DIR)/, $(CPP_OBJ_FILE))


all: $(CPP_OBJ)

$(OBJ_DIR)/%.o: %.cpp
	$(CPP_CC) $(INCLUDES) $< -o $(OBJ_DIR)/$@
	

clean: 
	rm *.o

