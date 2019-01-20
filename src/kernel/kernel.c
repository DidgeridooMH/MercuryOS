#include "../drivers/io.h"
#include "arch/x86/x86.h"
#include "interrupts/system_calls.h"
#include "interrupts/timer.h"
#include "../drivers/keyboard.h"
#include "../shell/shell.h"
#include "../runtime/itoa.h"
#include "memory/mmu.h"

int init() {
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

    paging_load();

    return 0;
}

void kmain() {
    io_clear_screen();
    io_printf("Mercury OS 0.0.2 Alpha\n\n");

    asm("sti");

    while(1) {
        shell_prompt();
    }
}
