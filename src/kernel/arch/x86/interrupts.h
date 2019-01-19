#ifndef INTTERUPTS_H
#define INTTERUPTS_H

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

void isr0();
void isr1();
void isr2();
void isr3();
void isr4();
void isr5();
void isr6();
void isr7();
void isr8();
void isr9();
void isr10();
void isr11();
void isr12();
void isr13();
void isr14();
void isr15();
void isr16();
void isr17();
void isr18();
void isr19();
void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();

void irq_handler(struct regs *r);

void(*irq_routines[16])(struct regs*) = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};

const char *exception_messages[] = {
  "Division by Zero",
  "Debug Exception",
  "Non Maskable Interrupt Exception",
  "Breakpoint Exception",
  "Into Detected Overflow Exception",
  "Out of Bounds Exception",
  "Invalid Opcode Exception",
  "No Coprocessor Exception",
  "Double Fault Exception",
  "Coprocessor Segment Overrun Exception",
  "Bad TSS Exception",
  "Segment Not Present Exception",
  "Stack Fault Exception",
  "General Protection Fault Exception",
  "Page Fault Exception",
  "Unknown Interrupt Exception",
  "Coprocessor Fault Exception",
  "Alignment Check Exception (486+)",
  "Machine Check Exception (Pentium/586+)",
  "Reserved"
};

const char *page_fault_messages[] = {
    "Supervisory process tried to read a non-present page entry",
    "Supervisory process tried to read a page and caused a protection fault",
    "Supervisory process tried to write to a non-present page entry",
    "Supervisory process tried to write a page and caused a protection fault",
    "User process tried to read a non-present page entry",
    "User process tried to read a page and caused a protection fault",
    "User process tried to write to a non-present page entry",
    "User process tried to write a page and caused a protection fault",
};

#endif
