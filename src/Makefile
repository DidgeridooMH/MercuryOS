C=i386-elf-gcc
ASM=nasm
FLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -masm=intel

all:
	make -C ./boot

	make -C ./drivers

	make -C ./kernel

	make -C ./runtime

	make -C ./shell
