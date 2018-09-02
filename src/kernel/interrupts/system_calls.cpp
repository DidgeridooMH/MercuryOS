#include "system_calls.h"
#include "../../drivers/io.h"
#include "../arch/x86/x86.h"

extern "C" void interrupt_test() {
    io.printf("software interrupt test: passed\n");
}

void system_calls_install() {
    idt_set_gate(50, reinterpret_cast<unsigned long>(&int32h), 0x08, 0x8E);
}
