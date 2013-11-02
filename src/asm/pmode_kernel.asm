USE32
SECTION .text

global _start_pmode_kernel
extern cursor_row, cursor_col, _cpp_kernel

_start_pmode_kernel:
	mov edx, 0xB8000
	mov byte [edx], 'A'
	xor ebx, ebx
	xor eax, eax
	mov al, [cursor_row]
	mov bl, 80*2
	mul bl
	add edx, eax
	xor eax, eax
	mov ah, [cursor_col]
	shl ax, 2
	add edx, eax
	; edx contient l'adresse en mémoire graphique ou sera écrit le prochain caractère
	
	cli;
	; on appelle le point d'entrée du noyau en C++
  call _cpp_kernel

  hlt





