all:
	@echo "Building object files..."
	make -C ./src
	ld -T src/linker.ld -m elf_i386 -o build/kernel.bin build/obj/*.o

run:
	@echo "Starting OS..."
	qemu-system-i386 -kernel build/kernel.bin

clean:
	rm build/obj/*.o
	rm build/kernel.bin
