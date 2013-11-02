
USE32

global _page_fault
global _general_protection_fault
global _irq_0
global _irq_1
global _system_calls
global _irq_12
global _irq_14
global _irq_15
global _default_interrupt

extern page_fault
extern general_protection_fault
extern irq_0
extern irq_1
extern system_calls
extern irq_12
extern irq_14
extern irq_15
extern default_interrupt

%macro SAVE_REGS 0
  pushad
  push ds
  push es
  push fs
  push gs
  push ebx
  mov bx, 0x10
  mov ds, bx
  pop ebx
%endmacro

%macro RESTORE_REGS 0
  pop gs
  pop fs
  pop es
  pop ds
  popad
%endmacro

%macro RESTORE_INT 0
  mov al,0x20
  out 0x20,al
%endmacro

%macro INTERRUPT 1
  SAVE_REGS
  call
  RESTORE_INT
  RESTORE_REGS
  iret
%endmacro




_page_fault:
  SAVE_REGS
  call page_fault
  RESTORE_INT
  RESTORE_REGS
  iret

_general_protection_fault:
  SAVE_REGS
  call page_fault
  RESTORE_INT
  RESTORE_REGS
  iret

_irq_0:
  SAVE_REGS
  call irq_0
  RESTORE_INT
  RESTORE_REGS
  iret

_irq_1:
  SAVE_REGS
  call irq_1
  RESTORE_INT
  RESTORE_REGS
  iret

_irq_12:
  SAVE_REGS
  call irq_12
  RESTORE_INT
  RESTORE_REGS
  iret

_irq_14:
  SAVE_REGS
  call irq_14
  RESTORE_INT
  RESTORE_REGS
  iret

_irq_15:
  SAVE_REGS
  call irq_15
  RESTORE_INT
  RESTORE_REGS
  iret

_default_interrupt:
  SAVE_REGS
  call default_interrupt
  RESTORE_INT
  RESTORE_REGS
  iret

 _system_calls:
  SAVE_REGS
  push ebx
  push eax
  call system_calls
  pop eax
  pop ebx
  RESTORE_INT
  RESTORE_REGS
  iret
