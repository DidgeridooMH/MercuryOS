#include "../drivers/io.h"
#include "../drivers/keyboard.h"
#include "../runtime/itoa.h"
#include "../shell/shell.h"
#include "arch/x86/x86.h"
#include "interrupts/system_calls.h"
#include "interrupts/timer.h"
#include "memory/mmu.h"

#define MAJOR_VERSION 0
#define MINOR_VERSION 2

#define STACK_ADDRESS 0xFFFF0000
#define STACK_LIMIT 0xFFFF0FF0

int kmain() {
    io_init();
    io_printf("Video context loaded\n");

    gdt_load(GDT_BASE);
    io_printf("GDT set\n");

    idt_load(IDT_BASE);
    io_printf("IDT populated\n");

    timer_install();
    timer_phase();
    io_printf("Timer has been initialized\n");

    keyboard_install();
    io_printf("Keyboard irq initialized\n");

    memory_init();
    io_printf("Memory allocation has been established...\n");

    io_clear_screen();
    io_set_color(TTY_COLOR_LIGHT_CYAN, TTY_COLOR_BLACK);
    io_printf(
        "===================================================\n"
        " _____                                _____  _____ \n"
        "|     | ___  ___  ___  _ _  ___  _ _ |     ||   __|\n"
        "| | | || -_||  _||  _|| | ||  _|| | ||  |  ||__   |\n"
        "|_|_|_||___||_|  |___||___||_|  |_  ||_____||_____|\n"
        "                                |___|              \n"
        "===================================================\n");
    io_printf("Version: %d.%d\n", MAJOR_VERSION, MINOR_VERSION);
    io_set_color(TTY_COLOR_WHITE, TTY_COLOR_BLACK);

    void *stack_ptr = allocate_frame();
    map_page(stack_ptr, STACK_ADDRESS, 2);

    asm("mov %%esp, %0" : : "dN"(STACK_LIMIT));

    asm("sti");

    while (1) {
        shell_prompt();
    }

    return 0;
}
