C=i386-elf-gcc
ASM=nasm
FLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -masm=intel
BUILD_PATH=../../build/obj

all: $(BUILD_PATH)/kernel.o
	make -C ./acpi
	make -C ./arch
	make -C ./interrupts
	make -C ./memory

$(BUILD_PATH)/kernel.o: kernel.c
	@echo "Compiling kernel.c"
	$(C) -c kernel.c -o $(BUILD_PATH)/kernel.o $(FLAGS)
