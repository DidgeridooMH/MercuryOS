all:
	@echo "Building object files..."
	make -C ./src
	i686-elf-gcc -T src/linker.ld -o build/mercuryos.bin -ffreestanding -O2 -nostdlib build/obj/*.o -lgcc

run:
	@echo "Starting OS..."
	qemu-system-i386 -kernel build/mercuryos.bin

clean:
	rm build/obj/*.o
	rm build/mercuryos.bin
