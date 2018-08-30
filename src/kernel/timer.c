#include "timer.h"

void timer_phase(int hz) {
  int divisor = TIMER_CLOCK_SPEED;
  outportb(TIMER_COMMAND_REGISTER, 0x36);
  outportb(TIMER_CHANNEL_0, divisor & 0xFF);
  outportb(TIMER_CHANNEL_0, divisor >> 8);
}

void timer_handler(struct regs *r) {
  timer_ticks++;
}

void timer_install() {
  irq_install_handler(0, timer_handler);
}

void timer_wait(int ticks) {
  unsigned long eticks = timer_ticks + ticks;
  while(timer_ticks < eticks);
}
