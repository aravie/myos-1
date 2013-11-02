
INCLUDES=	-I../includes \
					-I../libs/includes \
					-Iincludes \
					-I../mylibrary/includes

OBJ_DIR=../../release/obj


CPP_SRC=debug.cpp

CPP_OBJ_FILE=$(CPP_SRC:.cpp=.o)

CPP_OBJ=$(addprefix $(OBJ_DIR)/, $(CPP_OBJ_FILE))


all: $(CPP_OBJ)
	

$(OBJ_DIR)/%.o: %.cpp
	$(CPP_CC) $(INCLUDES) $< -o $@
	

clean: 
	rm $(OBJ_DIR)/*.o
