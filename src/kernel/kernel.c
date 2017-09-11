#include "kernel.h"
#include "../drivers/video/vgaterminal.h"

/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

void kmain(void) {
	/* Initialize terminal interface */
	terminal_initialize();

	/* Initialize GDT */
	terminal_writestring_s("Overwriting GDT...\n", VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
	gdt_install();

	/* Initialize IDT */

	terminal_writestring("\nProject Mercury Pre-Alpha Build\n\n[username]@[machinename]|/home: ");
}
