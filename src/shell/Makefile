C=i386-elf-gcc
ASM=nasm
FLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -masm=intel
BUILD_PATH=../../build/obj

all: $(BUILD_PATH)/shell.o

$(BUILD_PATH)/shell.o: shell.c shell.h
	@echo "Compiling shell.c"
	$(C) -c shell.c -o $(BUILD_PATH)/shell.o $(FLAGS)
