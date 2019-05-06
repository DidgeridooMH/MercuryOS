C=i386-elf-gcc
ASM=nasm
FLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -masm=intel
BUILD_PATH=../../../../build/obj

all: $(BUILD_PATH)/isr.o $(BUILD_PATH)/irq.o $(BUILD_PATH)/x86.o

$(BUILD_PATH)/isr.o: isr.S
	@echo "Compiling isr.s"
	$(ASM) -f elf32 -o $(BUILD_PATH)/isr.o isr.S

$(BUILD_PATH)/irq.o: irq.S
	@echo "Compiling irq.s"
	$(ASM) -f elf32 -o $(BUILD_PATH)/irq.o irq.S

$(BUILD_PATH)/x86.o: x86.c x86.h system.h
	@echo "Compiling x86.c"
	$(C) -c x86.c -o $(BUILD_PATH)/x86.o $(FLAGS)
