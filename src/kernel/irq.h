#ifndef IRQ_H
#define IRQ_H

#include "system.h"

void* irq_routines[16] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};

void outportb(unsigned short port, unsigned char data) {
  __asm__ __volatile__ ("outb %0, %1" : : "dN" (port), "a" (data));
}

void irq0();
void irq1();
void irq2();
void irq3();
void irq4();
void irq5();
void irq6();
void irq7();
void irq8();
void irq9();
void irq10();
void irq11();
void irq12();
void irq13();
void irq14();
void irq15();

void irq_install_handler(int irq, void (*handler)(struct regs *r));

void irq_uninstall_handler(int irq);

void irq_remap(void);

void irq_install();

void irq_handler(struct regs *r);

#endif
