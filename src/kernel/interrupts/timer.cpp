#include "timer.h"
#include "../../drivers/io.h"

unsigned long timer_ticks = 0;

void timer_phase() {
  int divisor = TIMER_CLOCK_SPEED;
  io.outportb(TIMER_COMMAND_REGISTER, 0x36);
  io.outportb(TIMER_CHANNEL_0, divisor & 0xFF);
  io.outportb(TIMER_CHANNEL_0, divisor >> 8);
}

void timer_handler(struct regs *r) {
  timer_ticks++;
}

void timer_install(x86* sys) {
  sys->irq_install_handler(0, timer_handler);
}

void timer_wait(int ticks) {
  unsigned long eticks = timer_ticks + ticks;
  while(timer_ticks < eticks);
}
