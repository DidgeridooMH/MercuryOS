CPP=i386-elf-g++
ASM=nasm
FLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -masm=intel -m32

all:
	@echo "Compiling multiboot.s"
	$(ASM) -f elf32 -o ../build/obj/boot.o boot/boot.S

	@echo "Compiling /kernel/arch/x86.s"
	$(ASM) -f elf32 -o ../build/obj/x86_s.o kernel/arch/x86.S

	@echo "Compiling idt_s.s"
	$(ASM) -f elf32 -o ../build/obj/idt_s.o kernel/idt_s.S

	@echo "Compiling idt_s.s"
	$(ASM) -f elf32 -o ../build/obj/isr_s.o kernel/isr_s.S

	@echo "Compiling idt_s.s"
	$(ASM) -f elf32 -o ../build/obj/isq_s.o kernel/irq_s.S

	@echo "Compiling system_calls.s"
	$(ASM) -f elf32 -o ../build/obj/system_calls_s.o kernel/system_calls_s.S

	@echo "Compiling kernel.cpp"
	$(CPP) -c kernel/kernel.cpp -o ../build/obj/kernel.o $(FLAGS)

	@echo "Compiling common.cpp"
	$(CPP) -c drivers/common.cpp -o ../build/obj/common.o $(FLAGS)

	@echo "Compiling vga.cpp"
	$(CPP) -c drivers/vga.cpp -o ../build/obj/vga.o $(FLAGS)

	@echo "Compiling string.cpp"
	$(CPP) -c runtime/string.cpp -o ../build/obj/string.o $(FLAGS)

	@echo "Compiling x86.cpp"
	$(CPP) -c kernel/arch/x86.cpp -o ../build/obj/x86.o $(FLAGS)

	@echo "Compiling memory.cpp"
	$(CPP) -c runtime/memory.cpp -o ../build/obj/memory.o $(FLAGS)

	@echo "Compiling itoa.cpp"
	$(CPP) -c runtime/itoa.cpp -o ../build/obj/itoa.o $(FLAGS)

	@echo "Compiling idt.cpp"
	$(CPP) -c kernel/idt.cpp -o ../build/obj/idt.o $(FLAGS)

	@echo "Compiling isr.cpp"
	$(CPP) -c kernel/isr.cpp -o ../build/obj/isr.o $(FLAGS)

	@echo "Compiling irq.cpp"
	$(CPP) -c kernel/irq.cpp -o ../build/obj/irq.o $(FLAGS)

	@echo "Compiling timer.cpp"
	$(CPP) -c kernel/timer.cpp -o ../build/obj/timer.o $(FLAGS)

	@echo "Compiling keyboard.cpp"
	$(CPP) -c drivers/keyboard.cpp -o ../build/obj/keyboard.o $(FLAGS)

	@echo "Compiling cursor.cpp"
	$(CPP) -c drivers/cursor.cpp -o ../build/obj/cursor.o $(FLAGS)

	@echo "Compiling shell.cpp"
	$(CPP) -c shell/shell.cpp -o ../build/obj/shell.o $(FLAGS)

	@echo "Compiling system_calls.cpp"
	$(CPP) -c kernel/system_calls.cpp -o ../build/obj/system_calls.o $(FLAGS)
