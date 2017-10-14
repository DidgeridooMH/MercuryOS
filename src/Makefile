C=i686-elf-gcc
ASM=nasm
FLAGS=-std=gnu99 -ffreestanding -O2 -Wno-all -masm=intel

all:
	@echo "Compiling multiboot.s"
	$(ASM) -f elf32 -o ../build/obj/boot.o boot/boot.S

	@echo "Compiling gtd_s.s"
	$(ASM) -f elf32 -o ../build/obj/gtd_s.o kernel/gtd_s.S

	@echo "Compiling kernel.c"
	$(C) -c kernel/kernel.c -o ../build/obj/kernel.o $(FLAGS)

	@echo "Compiling vga.c"
	$(C) -c drivers/vga.c -o ../build/obj/vga.o $(FLAGS)

	@echo "Compiling string.c"
	$(C) -c runtime/string.c -o ../build/obj/string.o $(FLAGS)

	@echo "Compiling gdt.c"
	$(C) -c kernel/gdt.c -o ../build/obj/gdt.o $(FLAGS)
