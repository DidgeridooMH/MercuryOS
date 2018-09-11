#ifndef SYSTEM_CALLS_H
#define SYSTEM_CALLS_H

#include "../arch/x86/x86.h"

extern "C" void int32h();

extern "C" void interrupt_test();

void system_calls_install(x86* sys);

#endif
