#include "system_calls.h"
#include "../../drivers/io.h"

extern "C" void interrupt_test() {
    Io::printf("software interrupt test: passed\n");
}

void system_calls_install(x86* sys) {
    sys->idt_set_gate(50, (unsigned)(int32h), 0x08, 0x8E);
}
