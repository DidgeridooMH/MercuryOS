#include "system_calls.h"

void interrupt_test() {
    printf("software interrupt test: passed\n");
}

void system_calls_install() {
    idt_set_gate(50, (unsigned)int32h, 0x08, 0x8E);
}
