[BITS 32]

global entry
extern kmain

MBALIGN  equ 1<<0
MEMINFO  equ 1<<1
FLAGS    equ MBALIGN | MEMINFO
MAGIC    equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

KERNEL_VIRTUAL_BASE equ 0xC0000000
KERNEL_PAGE_NUMBER  equ (KERNEL_VIRTUAL_BASE >> 22)

section .multiboot.data
align 4
MultiBootHeader:
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

STACKSIZE equ 16*1024

align 0x1000
BootPageDirectory:
    dd 0x00000083
    times (KERNEL_PAGE_NUMBER - 1) dd 0
    dd 0x00000083
    times (1024 - KERNEL_PAGE_NUMBER - 1) dd 0

section .multiboot.text progbits alloc exec nowrite align=16

entry:
    mov ecx, BootPageDirectory
    mov cr3, ecx

    mov ecx, cr4
    or ecx, 0x10
    mov cr4, ecx

    mov ecx, cr0
    or ecx, 0x80000000
    mov cr0, ecx

    jmp .StartInHigherHalf

section .text
.StartInHigherHalf:
    mov dword [BootPageDirectory], 0
    invlpg [0]

    mov esp, stack+STACKSIZE
    push eax
    add ebx, KERNEL_VIRTUAL_BASE
    push ebx
    call kmain

    cli
.endloop:
    hlt
    jmp .endloop

section .bss
align 32
stack:
    resb STACKSIZE
