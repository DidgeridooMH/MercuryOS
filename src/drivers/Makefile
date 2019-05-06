C=i386-elf-gcc
ASM=nasm
FLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -masm=intel
BUILD_PATH=../../build/obj

all: $(BUILD_PATH)/io.o $(BUILD_PATH)/keyboard.o

$(BUILD_PATH)/io.o: io.c io.h
	@echo "Compiling io.c"
	$(C) -c io.c -o $(BUILD_PATH)/io.o $(FLAGS)

$(BUILD_PATH)/keyboard.o: keyboard.c keyboard.h ../kernel/arch/x86/x86.h io.h ../runtime/memory.h
	@echo "Compiling keyboard.c"
	$(C) -c keyboard.c -o $(BUILD_PATH)/keyboard.o $(FLAGS)
