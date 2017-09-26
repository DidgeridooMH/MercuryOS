C=i686-elf-gcc
ASM=i686-elf-as
FLAGS=-std=gnu99 -ffreestanding -O2 -Wno-all

all:
	@echo "Compiling multiboot.s"
	$(ASM) boot/multiboot.s -o ../build/obj/multiboot.o

	@echo "Compiling kernel.c"
	$(C) -c kernel/kernel.c -o ../build/obj/kernel.o $(FLAGS)

	@echo "Compiling vgaterminal.c"
	$(C) -c drivers/video/vgaterminal.c -o ../build/obj/vgaterminal.o $(FLAGS)

	@echo "Compiling os.c"
	$(C) -c x86/os.c -o ../build/obj/os.o $(FLAGS)

	@echo "Compiling gdt.c"
	$(C) -c kernel/gdt.c -o ../build/obj/gdt.o $(FLAGS)

	@echo "Compiling idt.c"
	$(C) -c kernel/idt.c -o ../build/obj/idt.o $(FLAGS)

	@echo "Compiling isr.c"
	$(C) -c kernel/isr.c -o ../build/obj/isr.o $(FLAGS)
