C=i686-elf-gcc
ASM=nasm
FLAGS=-std=gnu99 -ffreestanding -O2 -Wno-all

all:
	@echo "Compiling multiboot.s"
	$(ASM) -f elf32 -o ../build/obj/boot.o boot/boot.S

	@echo "Compiling kernel.c"
	$(C) -c kernel/kernel.c -o ../build/obj/kernel.o $(FLAGS)

	@echo "Compiling vga.c"
	$(C) -c drivers/vga.c -o ../build/obj/vga.o $(FLAGS)

	@echo "Compiling string.c"
	$(C) -c runtime/string.c -o ../build/obj/string.o $(FLAGS)
