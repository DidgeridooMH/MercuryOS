#include "system_calls.h"
#include "../../drivers/io.h"

void system_interrupt_test() {
    io_printf("software interrupt test: passed\n");
}

void system_calls_install() {
    idt_set_gate(50, (unsigned)(int32h), 0x08, 0x8E);
}
