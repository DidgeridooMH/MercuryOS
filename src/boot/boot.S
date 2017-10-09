[BITS 32]
global start

extern kmain

start:
  mov esp, sys_stack
  call kmain

ALIGN 4
multiboot:
  MULTIBOOT_PAGE_ALIGN    equ 1<<0
  MULTIBOOT_MEMORY_INFO   equ 1<<1
  MULTIBOOT_AOUT_KLUDGE   equ 1<<16
  MULTIBOOT_HEADER_MAGIC  equ 0x1BADB002
  MULTIBOOT_HEADER_FLAGS  equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
  MULTIBOOT_CHECKSUM      equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
  EXTERN code, bss, end

  dd MULTIBOOT_HEADER_MAGIC
  dd MULTIBOOT_HEADER_FLAGS
  dd MULTIBOOT_CHECKSUM

  dd multiboot
  dd code
  dd bss
  dd end
  dd start

SECTION .bss
  resb 8192
sys_stack:
