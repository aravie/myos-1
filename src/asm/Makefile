
OBJ_DIR=../../release/obj
ASM_CC=nasm

ASM_OBJ_FILE=	pmode_kernel.o \
						rmode_kernel.o \
						interrupt_handlers.o \
						rmode_asm.o

ASM_OBJ=$(addprefix $(OBJ_DIR)/, $(ASM_OBJ_FILE))

all: $(OBJ_DIR)/bootsector.bin $(ASM_OBJ)

$(OBJ_DIR)/bootsector.bin: bootsector.asm
	$(ASM_CC) -f elf $< -o $(OBJ_DIR)/temp/bootsector.o
	ld -melf_i386 $(OBJ_DIR)/temp/bootsector.o -Ttext 0 -o $(OBJ_DIR)/temp/boot_elf.o
	objcopy -O binary $(OBJ_DIR)/temp/boot_elf.o $@

$(OBJ_DIR)/%.o: %.asm
	$(ASM_CC) -f elf $< -o $(OBJ_DIR)/$@ -ggdb
