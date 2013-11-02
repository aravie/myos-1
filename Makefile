
OBJ_DIR=release/obj

all: myos qemu

myos: build




build:
	make -C src
	cat $(OBJ_DIR)/bootsector.bin $(OBJ_DIR)/kernel.bin $< /dev/zero | dd of=myos bs=512 count=300

	
harddisk0:
	cat /dev/zero | dd of=$@ bs=512 count=10
	

usb: 
	dd if=myos of=/dev/sdb 

qemu:
	qemu -hda myos -m 512M
debug:	build
	qemu -hda myos -m 512M -s -S
	
read:
	od -A x -c harddisk0 | more

save: clean	.PHONY

.PHONY:
	./save

clean_obj:
	rm -rf $(OBJ_DIR)/*.o $(OBJ_DIR)/temp/*.o $(OBJ_DIR)/*.bin


clean: clean_obj
	rm -f myos
