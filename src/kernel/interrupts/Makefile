C=i386-elf-gcc
ASM=nasm
FLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -masm=intel
BUILD_PATH=../../../build/obj

all: $(BUILD_PATH)/system_calls_s.o $(BUILD_PATH)/timer.o $(BUILD_PATH)/system_calls.o

$(BUILD_PATH)/system_calls_s.o: system_calls.S  system_calls.h
	@echo "Compiling system_calls.s"
	$(ASM) -f elf32 -o $(BUILD_PATH)/system_calls_s.o system_calls.S

$(BUILD_PATH)/timer.o: timer.c timer.h ../arch/x86/x86.h ../../drivers/io.h
	@echo "Compiling timer.c"
	$(C) -c timer.c -o $(BUILD_PATH)/timer.o $(FLAGS)

$(BUILD_PATH)/system_calls.o: system_calls.c system_calls.h ../../drivers/io.h ../arch/x86/x86.h
	@echo "Compiling system_calls.c"
	$(C) -c system_calls.c -o $(BUILD_PATH)/system_calls.o $(FLAGS)
