[BITS 16]

%define 	KERNEL_BASE_SEGMENT		0x0800
%define 	BOOT_SECTOR_BASE_SEGMENT	0x07C0
%define 	STACK_SEGMENT 			0x00
%define 	STACK_OFFSET			0x6FFF
%define		NB_SECTORS_TO_COPY		128

global _start

_start:
jmp _boot_sector_start

%include "includes/utils.inc"

_boot_sector_start:
sti
mov ax, BOOT_SECTOR_BASE_SEGMENT
mov ds, ax
mov es, ax
mov ax, STACK_SEGMENT
mov ss, ax
mov sp, STACK_OFFSET

mov [boot_drive], dl

;mov ax, 0x02
;mov bx, 0x0
;mov dx, 0x0

; Dit bonjour
mov si, msg_welcome
call _print


push ds
mov ax, 0
mov ds, ax
mov si, 0x0475
lodsb
add al, '0'
mov ah, 0x0E
mov bh, 0x00
mov bl, 0x0F
int 0x10
pop ds
mov si, periph
call _print


; cacul la mémoire disponible et l'affiche
mov ax, 0xE801
int 0x15
shl ebx, 6		; on multiplie par 64. BX contient le nb de paquet de 64kb
add eax, ebx
call _convert_number_in_string
mov si, nb_str
call _print
mov si, memory
call _print


mov si, msg_kernel_loading
call _print

; on réinitialise les periphériques
xor ax, ax
int 0x13

push es
mov ax, KERNEL_BASE_SEGMENT
mov es, ax
mov bx, 0

mov ah, 0x02
mov al, NB_SECTORS_TO_COPY
mov ch, 0
mov cl, 2
mov dh, 0
mov dl, [boot_drive]
int 0x13
pop es

jnc jump_kernel

mov si, msg_erreur
call _print

hlt

jump_kernel:

xor ax, ax
mov al, [boot_drive]
push ax

jmp dword KERNEL_BASE_SEGMENT:0x0



; ***************************************** VARIABLES ******************************************



boot_drive		DB 0
msg_welcome 		DB "Welcome in MyOS", 13, 10, 0
periph 			DB " hard disks",13, 10, 0
memory 			DB " Memory Size (kb)", 13, 10, 0
msg_kernel_loading 	DB "The kernel is loading ...", 13, 10, 0
msg_erreur DB "Il y a eu une erreur au chargement du noyau", 10, 0
 


times 510-($-$$) db 144
dw 0xAA55
