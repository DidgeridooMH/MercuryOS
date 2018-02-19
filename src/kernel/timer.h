#ifndef TIMER_H
#define TIMER_H

#include "system.h"

#define TIMER_CLOCK_SPEED 1193180
#define TIMER_COMMAND_REGISTER 0x43
#define TIMER_CHANNEL_0 0x40

int timer_ticks = 0;

#endif
