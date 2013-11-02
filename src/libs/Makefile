

OBJ_DIR=../../release/obj

INCLUDES=	-I../includes \
					-Iincludes \
					-I../graphics/includes \
					-I../mylibrary/includes


CPP_SRC=	utils.cpp

CPP_OBJ_FILE=$(CPP_SRC:.cpp=.o)

CPP_OBJ=$(addprefix $(OBJ_DIR)/, $(CPP_OBJ_FILE))


all: $(CPP_OBJ)

$(OBJ_DIR)/%.o: %.cpp
	$(CPP_CC) $(INCLUDES) $< -o $(OBJ_DIR)/$@
	

clean: 
	rm *.o


