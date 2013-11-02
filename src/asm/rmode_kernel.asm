[SECTION .first_of_all]

USE16
global _start, _start_rmode_kernel, cursor_row, cursor_col, rgdt_ptr_rmode, rgdt_ptr, ivt_ptr, rmode_stack_top, rmode_stack_bottom
extern _start_pmode_kernel

%define STACK_OFFSET 0x6FFF

_start:
jmp _start_rmode_kernel

%include "includes/utils.inc"

_start_rmode_kernel:
xor ax, ax
pop ax
mov [boot_drive], al ; on récupère le code du periph qui a booté

; On sélectionne les bons segments
mov ax, 0x0
mov ds, ax
mov es, ax
; Stack Pointer
mov ax, 0x0
mov ss, ax
mov ax, STACK_OFFSET
mov sp, ax

mov si, msg_entry_point
call _print

; calcule la limite de GDT
xor eax, eax
xor ebx, ebx
mov ax, gdt_end  
mov bx, gdt_beginning
sub ax, bx
mov word [rgdt_ptr], ax

; calcule l'adresse lineaire de GDT
xor eax, eax
xor ebx, ebx
mov ax, ds
mov ecx, eax
shl ecx, 4
mov bx, gdt_beginning
add ecx, ebx
mov dword [rgdt_ptr+2], ecx

; Sauvegarde l'IDT du bios en 0x500 (espace libre jusqu'en 0x7C00)
;mov cx, 0x3ff
;_save_idt:
;mov bx, cx
;mov ax, [bx]
;mov [bx+BIOS_IDT], ax
;loop _save_idt
mov word [ivt_ptr], 0x3ff
mov dword [ivt_ptr+2], 0x0 ; LA IVT se trouve en 0x0
lidt [ivt_ptr]


mov si, msg_pmode_switch
call _print

; Get cursor position
mov ah, 0x03
mov bh, 0
int 0x10
mov byte [cursor_row], dh
mov byte [cursor_col], dl


;mov si, msg_press_a_key
;call _print
;xor ax, ax
;int 0x16
; On passe en mode protégé après appui sur une touche


; calcul la mémoire disponible
mov ax, 0xE801
int 0x15
shl ebx, 6    ; on multiplie par 64. BX contient le nb de paquet de 64kb
add eax, ebx
mov [entry_data], eax
mov ax, [boot_drive]
mov byte [entry_data+4], al;


; Passage en mode VESA 0x011A
;mov ax, 0x4F02
;mov bx, 0x011A
;int 0x10

; passage en modep
lgdt [rgdt_ptr]    ; charge la gdt
_switch_to_pmode
nop
nop

;push word [entry_data+4]
;push dword [entry_data]
jmp dword 0x8:_start_pmode_kernel    ; reinitialise le segment de code


hlt





msg_entry_point:	DB 	"Kernel entry point.", 13, 10, 0
msg_pmode_switch: 	DB 	"Kernel is switching to protected mode.", 13, 10, 0 
msg_press_a_key:	DB	"Press a key to enter kernel area...", 13, 10, 0

cursor_row:	DB 	0
cursor_col:	DB 	0
;HB = Row, LB = Col 


entry_data:
  memory_size dd 0
  boot_drive dw 0



ivt_ptr:
  dw 0
  dd 0

rgdt_ptr:
    dw 0  ; limite
    dd 0  ; base

rgdt_ptr_rmode:
    dw 0  ; limite
    dd 0  ; base


gdt_beginning:
    db 0, 0, 0, 0, 0, 0, 0, 0
gdt_cs:
    db 0xFF, 0xFF, 0x0, 0x0, 0x0, 10011011b, 11011111b, 0x0
gdt_ds:
    db 0xFF, 0xFF, 0x0, 0x0, 0x0, 10010011b, 11011111b, 0x0
gdt_end:




