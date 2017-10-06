#include "../x86/os.h"

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void *irq_routines[16] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};

void irq_install_handler(int irq, void (*handler)(struct regs *r)) {
  irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq) {
  irq_routines[irq] = 0;
}

void irq_remap(void) {
  outb(0x20, 0x11);
  outb(0xA0, 0x11);
  outb(0x21, 0x20);
  outb(0xA1, 0x28);
  outb(0x21, 0x04);
  outb(0xA1, 0x02);
  outb(0x21, 0x01);
  outb(0xA1, 0x01);
  outb(0x21, 0x0);
  outb(0xA1, 0x0);
}

void irq_install() {
  irq_remap();

  idt_set_gate(0x20, (unsigned)irq0, 0x08, 0x8E);
  idt_set_gate(0x20, (unsigned)irq0, 0x08, 0x8E);
  idt_set_gate(0x20, (unsigned)irq0, 0x08, 0x8E);
  idt_set_gate(0x20, (unsigned)irq0, 0x08, 0x8E);
  idt_set_gate(0x20, (unsigned)irq0, 0x08, 0x8E);
  idt_set_gate(0x20, (unsigned)irq0, 0x08, 0x8E);
  idt_set_gate(0x20, (unsigned)irq0, 0x08, 0x8E);
  idt_set_gate(0x20, (unsigned)irq0, 0x08, 0x8E);
  idt_set_gate(0x20, (unsigned)irq0, 0x08, 0x8E);
  idt_set_gate(0x20, (unsigned)irq0, 0x08, 0x8E);
  idt_set_gate(0x20, (unsigned)irq0, 0x08, 0x8E);
  idt_set_gate(0x20, (unsigned)irq0, 0x08, 0x8E);
  idt_set_gate(0x20, (unsigned)irq0, 0x08, 0x8E);
  idt_set_gate(0x20, (unsigned)irq0, 0x08, 0x8E);
  idt_set_gate(0x20, (unsigned)irq0, 0x08, 0x8E);
  idt_set_gate(0x20, (unsigned)irq0, 0x08, 0x8E);
}

void irq_handler(struct regs *r) {
  void (*handler)(struct regs*r);

  handler = irq_routines[r->int_no - 32];
  if(handler) {
    handler(r);
  }

  if(r->int_no >= 40) {
    outb(0xA0, 0x20);
  }

  outb(0x20, 0x20);
}
