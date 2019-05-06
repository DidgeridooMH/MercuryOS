C=i386-elf-gcc
ASM=nasm
FLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -masm=intel
BUILD_PATH=../../build/obj

all: $(BUILD_PATH)/string.o $(BUILD_PATH)/memory.o $(BUILD_PATH)/itoa.o

$(BUILD_PATH)/string.o: string.c string.h
	@echo "Compiling string.c"
	$(C) -c string.c -o $(BUILD_PATH)/string.o $(FLAGS)

$(BUILD_PATH)/memory.o: memory.c memory.h
	@echo "Compiling memory.c"
	$(C) -c memory.c -o $(BUILD_PATH)/memory.o $(FLAGS)

$(BUILD_PATH)/itoa.o: itoa.c itoa.h
	@echo "Compiling itoa.c"
	$(C) -c itoa.c -o $(BUILD_PATH)/itoa.o $(FLAGS)
