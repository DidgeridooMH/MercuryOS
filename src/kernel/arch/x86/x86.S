[BITS 32]

global gdt_set
global idt_set

extern gdt
extern idt

gdt_set:
  lgdt [gdt]
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  jmp 0x08:reload_CS

reload_CS:
  ret

idt_set:
    lidt [idt]
    ret
