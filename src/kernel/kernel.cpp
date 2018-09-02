#include "../drivers/io.h"
#include "arch/x86/x86.h"
#include "arch/x86/irq.h"
#include "arch/x86/isr.h"
#include "interrupts/system_calls.h"
#include "interrupts/timer.h"
#include "../drivers/keyboard.h"
#include "../shell/shell.h"
#include "../runtime/itoa.h"

extern "C" int kmain() {
    Io io;
    io.printf("Video context loaded\n");

    gdt_load();
    io.printf("GDT set\n");

    idt_load();
    io.printf("IDT populated\n");

    system_calls_install();
    io.printf("System Calls Installed\n");

    timer_install();
    timer_phase(100);
    io.printf("Timer has been initialized\n");

    keyboard_install();
    io.printf("Keyboard irq initialized\n");

    //io.clearScreen();
    io.printf("Mercury OS 0.0.2 Alpha\n\n");

    asm("sti");

    while(1) {
        shell_prompt(&io);
    }

    return 0;
}
