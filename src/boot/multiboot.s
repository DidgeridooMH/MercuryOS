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
    jmp $0x08, $flush2
flush2:
    ret

.size _start, . - _start
