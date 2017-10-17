[BITS 32]

global idt_set
extern idt
idt_set:
  lidt [idt]
  ret
