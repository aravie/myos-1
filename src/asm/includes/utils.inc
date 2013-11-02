
; Affiche le msg pointé par ds:si
_print:
	push ax
	push bx
	push ds
	push si
	mov ah, 0x0E ; function E
	mov bh, 0x00  ; Page 0
	mov bl, 0x0F ; White font on black background
	_print_begin:
	lodsb         ; mov ds:si -> al
	cmp al, 0    ; End of string ?
	je _end_print
	int 0x10
	jmp _print_begin
	_end_print:
	pop si
	pop ds
	pop bx
	pop ax
ret




; convertit le nombre stocké dans ax et le met dans la variable nb_str
_convert_number_in_string:
	push eax
	push ebx
	push ecx

  mov bx, 0
  mov ecx, 14
  init_nb_str:
  mov byte [nb_str+bx], 0
  inc bx
  loop init_nb_str

	mov ecx, 0		; CX est un registre temporaire
	mov ebx, 0

	_first_test:
	cmp eax, 10 		; on test si ax est inférieur à 10
	jb _store_figure	; s'il est inférieur, on saute
	sub eax, 10		; sinon on décrémente ax de 10
	inc ecx			; on incremente cx de 1, cx contiendra le nb de 10aines
	jmp _first_test		; et on boucle
	_store_figure:		
	add ax, '0'
	push ax
	inc bx			; bx contient le nombre de chiffres à la fin

	mov eax, ecx
	mov ecx, 0

	test ax, ax
	jnz _first_test

	mov ecx, 0
	mov cx, bx
	mov bx, 0
	_fill_string:
	pop ax
	mov [nb_str+bx], al
	inc bx
	loop _fill_string

	pop ecx
	pop ebx
	pop eax
ret

nb_str 			DB  0,0,0,0,0,0,0,0,0,0,0,0



; Attention cette macro désactive les interruptions, il faudra les réactiver plus tard
%macro _switch_to_pmode 0
	cli
	mov eax, cr0
	or  ax, 1
	mov cr0, eax

	mov ax, 0x10        ; segment de donne
	mov ds, ax
	mov fs, ax
	mov gs, ax
	mov es, ax
	mov ax, 0x10
	mov ss, ax
	mov esp, 0x6FFF
%endmacro


