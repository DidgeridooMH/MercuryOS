all:
	@echo "Compiling multiboot.s"
	i686-elf-as boot/multiboot.s -o ../build/obj/multiboot.o

	@echo "Compiling kernel.c"
	i686-elf-gcc -c kernel/kernel.c -o ../build/obj/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

	@echo "Compiling vgaterminal.c"
	i686-elf-gcc -c drivers/video/vgaterminal.c -o ../build/obj/vgaterminal.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

	@echo "Compiling os.c"
	i686-elf-gcc -c x86/os.c -o ../build/obj/os.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

	@echo "Compiling gdt.c"
	i686-elf-gcc -c kernel/gtd.c -o ../build/obj/gtd.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
