C=i386-elf-gcc
ASM=nasm
FLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -masm=intel

all:
	@echo "Compiling multiboot.s"
	$(ASM) -f elf32 -o ../build/obj/boot.o boot/boot.S

	@echo "Compiling idt_s.s"
	$(ASM) -f elf32 -o ../build/obj/isr_s.o kernel/arch/x86/isr.S

	@echo "Compiling idt_s.s"
	$(ASM) -f elf32 -o ../build/obj/isq_s.o kernel/arch/x86/irq.S

	@echo "Compiling system_calls.s"
	$(ASM) -f elf32 -o ../build/obj/system_calls_s.o kernel/interrupts/system_calls.S

	@echo "Compiling io.c"
	$(C) -c drivers/io.c -o ../build/obj/io.o $(FLAGS)

	@echo "Compiling keyboard.c"
	$(C) -c drivers/keyboard.c -o ../build/obj/keyboard.o $(FLAGS)

	@echo "Compiling x86.c"
	$(C) -c kernel/arch/x86/x86.c -o ../build/obj/x86.o $(FLAGS)

	@echo "Compiling timer.c"
	$(C) -c kernel/interrupts/timer.c -o ../build/obj/timer.o $(FLAGS)

	@echo "Compiling kernel.c"
	$(C) -c kernel/kernel.c -o ../build/obj/kernel.o $(FLAGS)

	@echo "Compiling string.c"
	$(C) -c runtime/string.c -o ../build/obj/string.o $(FLAGS)

	@echo "Compiling memory.c"
	$(C) -c runtime/memory.c -o ../build/obj/memory.o $(FLAGS)

	@echo "Compiling itoa.c"
	$(C) -c runtime/itoa.c -o ../build/obj/itoa.o $(FLAGS)

	@echo "Compiling shell.c"
	$(C) -c shell/shell.c -o ../build/obj/shell.o $(FLAGS)

	@echo "Compiling system_calls.c"
	$(C) -c kernel/interrupts/system_calls.c -o ../build/obj/system_calls.o $(FLAGS)

	@echo "Compiling mmu.c"
	$(C) -c kernel/memory/mmu.c -o ../build/obj/mmu.o $(FLAGS)

	@echo "Compiling heap_allocator.c"
	$(C) -c kernel/memory/heap_allocator.c -o ../build/obj/heap_allocator.o $(FLAGS)
