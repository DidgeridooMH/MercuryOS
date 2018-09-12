#include "../drivers/io.h"
#include "arch/x86/x86.h"
#include "interrupts/system_calls.h"
#include "interrupts/timer.h"
#include "../drivers/keyboard.h"
#include "../shell/shell.h"
#include "../runtime/itoa.h"

extern "C" int kmain() {
    x86 sys;

    Io::init();
    Io::printf("Video context loaded\n");

    sys.gdt_load();
    Io::printf("GDT set\n");

    sys.idt_load();
    Io::printf("IDT populated\n");

    system_calls_install(&sys);
    Io::printf("System Calls Installed\n");

    sys.idt_set();

    timer_install(&sys);
    timer_phase();
    Io::printf("Timer has been initialized\n");

    keyboard_install(&sys);
    Io::printf("Keyboard irq initialized\n");

    Io::clearScreen();
    Io::printf("Mercury OS 0.0.2 Alpha\n\n");

    asm("sti");

    while(1) {
        shell_prompt();
    }

    return 0;
}
