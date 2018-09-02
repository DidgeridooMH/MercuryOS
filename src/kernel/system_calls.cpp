#include "system_calls.h"
#include "../drivers/vga.h"
#include "idt.h"

extern "C" void interrupt_test() {
    printf("software interrupt test: passed\n");
}

void system_calls_install() {
    idt_set_gate(50, reinterpret_cast<unsigned long>(&int32h), 0x08, 0x8E);
}
