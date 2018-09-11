CPP=i386-elf-g++
ASM=nasm
FLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -masm=intel -m32

all:
	@echo "Compiling multiboot.s"
	$(ASM) -f elf32 -o ../build/obj/boot.o boot/boot.S

	@echo "Compiling idt_s.s"
	$(ASM) -f elf32 -o ../build/obj/isr_s.o kernel/arch/x86/isr.S

	@echo "Compiling idt_s.s"
	$(ASM) -f elf32 -o ../build/obj/isq_s.o kernel/arch/x86/irq.S

	@echo "Compiling system_calls.s"
	$(ASM) -f elf32 -o ../build/obj/system_calls_s.o kernel/interrupts/system_calls.S

	@echo "Compiling kernel.cpp"
	$(CPP) -c kernel/kernel.cpp -o ../build/obj/kernel.o $(FLAGS)

	@echo "Compiling vga.cpp"
	$(CPP) -c drivers/io.cpp -o ../build/obj/io.o $(FLAGS)

	@echo "Compiling string.cpp"
	$(CPP) -c runtime/string.cpp -o ../build/obj/string.o $(FLAGS)

	@echo "Compiling x86.cpp"
	$(CPP) -c kernel/arch/x86/x86.cpp -o ../build/obj/x86.o $(FLAGS)

	@echo "Compiling memory.cpp"
	$(CPP) -c runtime/memory.cpp -o ../build/obj/memory.o $(FLAGS)

	@echo "Compiling itoa.cpp"
	$(CPP) -c runtime/itoa.cpp -o ../build/obj/itoa.o $(FLAGS)

	@echo "Compiling timer.cpp"
	$(CPP) -c kernel/interrupts/timer.cpp -o ../build/obj/timer.o $(FLAGS)

	@echo "Compiling keyboard.cpp"
	$(CPP) -c drivers/keyboard.cpp -o ../build/obj/keyboard.o $(FLAGS)

	@echo "Compiling shell.cpp"
	$(CPP) -c shell/shell.cpp -o ../build/obj/shell.o $(FLAGS)

	@echo "Compiling system_calls.cpp"
	$(CPP) -c kernel/interrupts/system_calls.cpp -o ../build/obj/system_calls.o $(FLAGS)

	@echo "Compiling interface.cpp"
	$(CPP) -c kernel/interface.cpp -o ../build/obj/interface.o $(FLAGS)
