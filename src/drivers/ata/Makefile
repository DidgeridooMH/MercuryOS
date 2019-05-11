C=i386-elf-gcc
ASM=nasm
FLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -masm=intel
BUILD_PATH=../../../build/obj

all: $(BUILD_PATH)/ata.o

$(BUILD_PATH)/ata.o: ata.c ata.h ../io.h
	@echo "Compiling ata.c"
	$(C) -c ata.c -o $(BUILD_PATH)/ata.o $(FLAGS)
