#ifndef TIMER_H
#define TIMER_H

#include "../arch/x86/x86.h"

#define TIMER_CLOCK_SPEED 1193180
#define TIMER_COMMAND_REGISTER 0x43
#define TIMER_CHANNEL_0 0x40

void timer_phase();
void timer_handler(struct regs *r);
void timer_install(x86* sys);
void timer_wait(int ticks);

#endif
