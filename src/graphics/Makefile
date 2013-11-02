

INCLUDES=	-I../includes \
					-I../libs/includes \
					-Iincludes \
					-I../debug/includes \
					-I../mylibrary/includes \
					-I../memory/includes \
					-I../kernel/includes

OBJ_DIR=../../release/obj


CPP_SRC=	vga.cpp \
					vesa.cpp \
					cursor.cpp \
					drawer.cpp \

CPP_OBJ_FILE=$(CPP_SRC:.cpp=.o)

CPP_OBJ=$(addprefix $(OBJ_DIR)/, $(CPP_OBJ_FILE))


all: $(CPP_OBJ)

$(OBJ_DIR)/%.o: %.cpp
	$(CPP_CC) $(INCLUDES) $< -o $(OBJ_DIR)/$@
	

clean: 
	rm *.o