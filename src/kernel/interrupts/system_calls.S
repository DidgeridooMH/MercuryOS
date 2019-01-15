[BITS 32]

%macro INT_SETUP 1
    cli
    push 0
    push %1

    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
%endmacro

%macro INT_RESTORE 0
  pop eax
  pop gs
  pop fs
  pop es
  pop ds
  popa
  add esp, 8
  iret
%endmacro


extern system_interrupt_test

global int32h

; Write IO system call
int32h:
    INT_SETUP 0x32

    call system_interrupt_test

    INT_RESTORE
