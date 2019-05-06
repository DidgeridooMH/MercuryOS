ASM=nasm
FLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -masm=intel
BUILD_PATH=../../build/obj

all: $(BUILD_PATH)/boot.o

$(BUILD_PATH)/boot.o: boot.S ../kernel/kernel.c
	@echo "Compiling boot.s"
	$(ASM) -f elf32 -o $(BUILD_PATH)/boot.o boot.S
