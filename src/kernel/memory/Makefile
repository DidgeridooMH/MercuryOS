C=i386-elf-gcc
ASM=nasm
FLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -masm=intel
BUILD_PATH=../../../build/obj

all: $(BUILD_PATH)/mmu.o $(BUILD_PATH)/paging.o $(BUILD_PATH)/frame.o

$(BUILD_PATH)/mmu.o: mmu.c mmu.h ../../drivers/io.h ../../runtime/itoa.h ../../runtime/memory.h ../arch/x86/x86.h
	@echo "Compiling mmu.c"
	$(C) -c mmu.c -o $(BUILD_PATH)/mmu.o $(FLAGS)

$(BUILD_PATH)/paging.o: paging.c paging.h 
	@echo "Compiling paging.c"
	$(C) -c paging.c -o $(BUILD_PATH)/paging.o $(FLAGS)

$(BUILD_PATH)/frame.o: frame.c frame.h mmu.h paging.h
	@echo "Compiling frame.c"
	$(C) -c frame.c -o $(BUILD_PATH)/frame.o $(FLAGS)

