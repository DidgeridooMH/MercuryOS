C=gcc
ASM=nasm
FLAGS=-std=gnu99 -m32 -ffreestanding -O2 -Wno-all -masm=intel -fno-pie

all:
	@echo "Compiling multiboot.s"
	$(ASM) -f elf32 -o ../build/obj/boot.o boot/boot.S

	@echo "Compiling gtd_s.s"
	$(ASM) -f elf32 -o ../build/obj/gtd_s.o kernel/gtd_s.S

	@echo "Compiling idt_s.s"
	$(ASM) -f elf32 -o ../build/obj/idt_s.o kernel/idt_s.S

	@echo "Compiling idt_s.s"
	$(ASM) -f elf32 -o ../build/obj/isr_s.o kernel/isr_s.S

	@echo "Compiling idt_s.s"
	$(ASM) -f elf32 -o ../build/obj/isq_s.o kernel/irq_s.S

	@echo "Compiling system_calls.s"
	$(ASM) -f elf32 -o ../build/obj/system_calls_s.o kernel/system_calls_s.S

	@echo "Compiling kernel.c"
	$(C) -c kernel/kernel.c -o ../build/obj/kernel.o $(FLAGS)

	@echo "Compiling common.c"
	$(C) -c drivers/common.c -o ../build/obj/common.o $(FLAGS)

	@echo "Compiling vga.c"
	$(C) -c drivers/vga.c -o ../build/obj/vga.o $(FLAGS)

	@echo "Compiling string.c"
	$(C) -c runtime/string.c -o ../build/obj/string.o $(FLAGS)

	@echo "Compiling gdt.c"
	$(C) -c kernel/gdt.c -o ../build/obj/gdt.o $(FLAGS)

	@echo "Compiling memory.c"
	$(C) -c runtime/memory.c -o ../build/obj/memory.o $(FLAGS)

	@echo "Compiling idt.c"
	$(C) -c kernel/idt.c -o ../build/obj/idt.o $(FLAGS)

	@echo "Compiling isr.c"
	$(C) -c kernel/isr.c -o ../build/obj/isr.o $(FLAGS)

	@echo "Compiling irq.c"
	$(C) -c kernel/irq.c -o ../build/obj/irq.o $(FLAGS)

	@echo "Compiling timer.c"
	$(C) -c kernel/timer.c -o ../build/obj/timer.o $(FLAGS)

	@echo "Compiling keyboard.c"
	$(C) -c drivers/keyboard.c -o ../build/obj/keyboard.o $(FLAGS)

	@echo "Compiling cursor.c"
	$(C) -c drivers/cursor.c -o ../build/obj/cursor.o $(FLAGS)

	@echo "Compiling shell.c"
	$(C) -c shell/shell.c -o ../build/obj/shell.o $(FLAGS)

	@echo "Compiling system_calls.c"
	$(C) -c kernel/system_calls.c -o ../build/obj/system_calls.o $(FLAGS)
