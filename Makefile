LDFLAGS=-T src/linker.ld -ffreestanding -O2 -nostdlib -lgcc

all:
	@echo "Building object files..."
	make -C ./src
	i386-elf-gcc $(LDFLAGS) -o build/kernel.bin build/obj/*.o
	mkdir -p build/isodir/boot/grub
	cp build/kernel.bin build/isodir/boot/
	cp src/grub.cfg build/isodir/boot/grub/grub.cfg
	grub-mkrescue -o build/mos.iso build/isodir

run-qemu:
	@echo "Starting os..."
	qemu-system-i386 -cdrom build/mos.iso

run-bochs:
	@echo "Starting OS..."
	bochs -f bochs.x86_64

clean:
	rm build/obj/*.o
	rm build/kernel.bin
	rm -r build/isodir
	rm build/mos.iso
