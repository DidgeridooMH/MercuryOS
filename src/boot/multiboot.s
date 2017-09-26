/* Declare the multiboot header */
.set ALIGN,   1<<0
.set MEMINFO, 1<<1
.set FLAGS,   ALIGN | MEMINFO
.set MAGIC,   0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text
.global _start
.type _start, @function
_start:

    /* Setup stack register */
    mov $stack_top, %esp

    call kmain

    cli
1:  hlt
    jmp 1b

.global gdt_flush
.extern gp
gdt_flush:
    lgdt (gp)
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ljmp $0x08, $flush2
flush2:
    ret

.global idt_load
.extern idtp
idt_load:
    lidt (idtp)
    ret

.global isr0
.global isr1
.global isr2
.global isr3
.global isr4
.global isr5
.global isr6
.global isr7
.global isr8
.global isr9
.global isr10
.global isr11
.global isr12
.global isr13
.global isr14
.global isr15
.global isr16
.global isr17
.global isr18
.global isr19
.global isr20
.global isr21
.global isr22
.global isr23
.global isr24
.global isr25
.global isr26
.global isr27
.global isr28
.global isr29
.global isr30
.global isr31

/*Divide by Zero Exception*/
/*Push Error Code/Dummy and Exception respectively */
isr0:
  cli
  push $0x0
  push $0x0
  jmp isr_common_stub

isr1:
  cli
  push $0
  push $1
  jmp isr_common_stub

isr2:
  cli
  push $0
  push $2
  jmp isr_common_stub

isr3:
  cli
  push $0
  push $3
  jmp isr_common_stub

isr4:
  cli
  push $0
  push $4
  jmp isr_common_stub

isr5:
  cli
  push $0
  push $5
  jmp isr_common_stub

isr6:
  cli
  push $0
  push $6
  jmp isr_common_stub

isr7:
  cli
  push $0
  push $7
  jmp isr_common_stub

isr8:
  cli
  push $8
  jmp isr_common_stub

isr9:
  cli
  push $0
  push $9
  jmp isr_common_stub

isr10:
  cli
  push $0x0A
  jmp isr_common_stub

isr11:
  cli
  push $0x0B
  jmp isr_common_stub

isr12:
  cli
  push $0x0C
  jmp isr_common_stub

isr13:
  cli
  push $0x0D
  jmp isr_common_stub

isr14:
  cli
  push $0x0E
  jmp isr_common_stub

isr15:
  cli
  push $0x0F
  jmp isr_common_stub

isr16:
  cli
  push $0x10
  jmp isr_common_stub

isr17:
  cli
  push $0x11
  jmp isr_common_stub

isr18:
  cli
  push $0x12
  jmp isr_common_stub

isr19:
  cli
  push $0x13
  jmp isr_common_stub

isr20:
  cli
  push $0x14
  jmp isr_common_stub

isr21:
  cli
  push $0x15
  jmp isr_common_stub

isr22:
  cli
  push $0x16
  jmp isr_common_stub

isr23:
  cli
  push $0x17
  jmp isr_common_stub

isr24:
  cli
  push $0x18
  jmp isr_common_stub

isr25:
  cli
  push $0x19
  jmp isr_common_stub

isr26:
  cli
  push $0x1A
  jmp isr_common_stub

isr27:
  cli
  push $0x1B
  jmp isr_common_stub

isr28:
  cli
  push $0x1C
  jmp isr_common_stub

isr29:
  cli
  push $0x1D
  jmp isr_common_stub

isr30:
  cli
  push $0x1E
  jmp isr_common_stub

isr31:
  cli
  push $0x1F
  jmp isr_common_stub

.extern fault_handler

isr_common_stub:
  pusha
  push %ds
  push %es
  push %fs
  push %gs

  mov $0x10, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs
  mov %esp, %eax

  push %eax
  mov $fault_handler, %eax
  call %eax
  pop %eax
  pop %gs
  pop %fs
  pop %es
  pop %ds
  popa
  add $0x08, %esp
  iret

.size _start, . - _start
