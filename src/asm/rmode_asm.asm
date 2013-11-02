
; ATTENTION, cette section doit se trouver dans les premières adresses de la mémoire car, elle accède à des zones mémoire en mode réel !!!

[SECTION .rmode]

USE32

global rmode_int
extern ivt_ptr, idtptr

;%define RMODE_STACK_OFFSET 0x6FFE


rmode_int:

push eax
; On sauvegarde le paramètre dans int_num
mov eax, [esp+8]
and eax, 0x0000FFFF
mov [int_num], eax
;mov dword [int_num], 0x10

pop eax

; on enregistre l'état du rflags pour récupérer l'état des interruptions
pushf
cli
nop
nop

pushad
push di
push es
push si

; sauvegarde les registres indispensables à recharger en sortie de mode reel
mov eax, cr0
push eax
mov eax, cr3
push eax
mov [esp_var], esp


mov eax, cr0
; on désactive le paging
and eax, 0x7FFFFFFF
mov cr0, eax

xor eax, eax
mov cr3, eax ; on met cr3 à 0

mov ax, 0x28
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax
mov esp, 0x6FF2
lidt [ivt_ptr]

jmp 0x20:int_enter_pmode
int_enter_pmode:
USE16

mov eax, cr0
; on passe en mode réel
and al, ~1
mov cr0, eax
mov ax, 0x0
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax
mov sp, 0x6FF2
jmp 0x0:int_rmode

int_rmode:

xor eax, eax
xor ebx, ebx
xor ecx, ecx
xor edx, edx

pop si
pop di
pop es
pop dx
pop cx
pop bx
;pop ax
sti

mov eax, [int_num]
cmp eax, 0x10
je int10h
cmp eax, 0x13
je int13h
jmp next_int

int10h:
xor eax, eax
pop ax
int 0x10
jmp next_int

int13h:
xor eax, eax
pop ax
int 0x13
; on effectue vraiment le travail de l'interruption en rmode

next_int:
push ax
push bx
push cx
push dx
push es
push di
push si
;push ds

cli

; on repasse en mode protégé
mov eax, cr0
or al, 0x01
mov cr0, eax
mov ax, 0x10
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ax, 0x18
mov ss, ax
jmp dword 0x08:int_return_pmode

int_return_pmode:
USE32

lidt [idtptr]
mov esp, [esp_var]
pop eax
mov cr3, eax
pop eax
mov cr0, eax
pop si
pop es
pop di
popad
popf

ret



int_num dd 0
esp_var dd 0

