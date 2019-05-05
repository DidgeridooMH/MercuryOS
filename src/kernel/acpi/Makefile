C=i386-elf-gcc
ASM=nasm
FLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -masm=intel
BUILD_PATH=../../../build/obj

all: $(BUILD_PATH)/acpi.o

$(BUILD_PATH)/acpi.o: acpi.c acpi.h ../../drivers/io.h ../memory/mmu.h ../../runtime/itoa.h ../../runtime/string.h
	@echo "Compiling acpi.c"
	$(C) -c acpi.c -o $(BUILD_PATH)/acpi.o $(FLAGS)
