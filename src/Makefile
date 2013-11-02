
export CPP_CC=g++ -c -Wall -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-rtti -fno-stack-protector -fno-threadsafe-statics -m32 -ggdb

ROOT_DIR=.

OBJ_DIR=../release/obj

INCLUDES=	-Iincludes \
					-Ilibs/includes \
					-Ikernel_tools/includes \
					-Igraphics/includes

SRC_DIRS=	graphics \
				libs		\
				rmode 


all:
	make -C asm
	make -C graphics
	make -C libs
	make -C rmode
	make -C kernel
	make -C mylibrary
	make -C interrupts
	make -C memory
	make -C debug
	make -C devices
	make -C file
	ld  -melf_i386 $(OBJ_DIR)/*.o -Tlinker.ld -o $(OBJ_DIR)/temp/kernel_elf.o -M > map.log
	objcopy -O binary $(OBJ_DIR)/temp/kernel_elf.o $(OBJ_DIR)/kernel.bin


clean: 
	rm *.o

