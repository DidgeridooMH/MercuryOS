C=i386-elf-gcc
ASM=nasm
FLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -masm=intel
BUILD_PATH=../../../build/obj

all: $(BUILD_PATH)/ata.o $(BUILD_PATH)/atapi.o

$(BUILD_PATH)/ata.o: ata.c ata.h ../io.h ../../kernel/memory/mmu.h
	@echo "Compiling ata.c"
	$(C) -c ata.c -o $(BUILD_PATH)/ata.o $(FLAGS)

$(BUILD_PATH)/atapi.o: atapi.c atapi.h ../io.h ata.h
	@echo "Compiling atapi.c"
	$(C) -c atapi.c -o $(BUILD_PATH)/atapi.o $(FLAGS)
