#include "x86.h"
#include "interrupts.h"
#include "system.h"
#include "../../../drivers/io.h"
#include "../../../runtime/memory.h"
#include "../../../runtime/itoa.h"

struct gdt_descriptor gdt;
struct idt_descriptor idt;

void x86::gdt_set_entry( int id,
                    unsigned long base,
                    unsigned long limit,
                    unsigned char access,
                    unsigned char flags) {
    gdt_entries[id].base_low = base & 0xFFFF;
    gdt_entries[id].base_middle = (base >> 16) & 0xFF;
    gdt_entries[id].base_high = (base >> 24) & 0xFF;

    gdt_entries[id].limit = limit & 0xFFFF;

    gdt_entries[id].granularity = (limit >> 16) & 0xF;
    gdt_entries[id].granularity |= (flags & 0xF0);

    gdt_entries[id].access = access;
}

void x86::gdt_load() {
    gdt.size = sizeof(struct gdt_entry) * 7;
    gdt.offset = GDT_BASE;

    gdt_set_entry(0, 0, 0, 0, 0);
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9B, 0xCF);
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x93, 0xCF);
    gdt_set_entry(3, 0, 0x00000000, 0x97, 0xCF);
    gdt_set_entry(4, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    gdt_set_entry(5, 0, 0xFFFFFFFF, 0xF2, 0xCF);
    gdt_set_entry(6, 0, 0x00000000, 0xF7, 0xCF);

    memcpy((char*)gdt.offset, (char*)gdt_entries, gdt.size);

    asm("lgdt [gdt]         \n \
        mov ax, 0x10        \n \
        mov ds, ax          \n \
        mov es, ax          \n \
        mov fs, ax          \n \
        mov gs, ax          \n \
        jmp 0x08:reload_CS  \n \
        reload_CS:          \n");
}

void x86::idt_load() {
  idt.limit = sizeof(struct idt_entry) * 0xFF;
  idt.base = IDT_BASE;

  memset((char*)idt_entries, 0, sizeof(struct idt_entry) * 256);

  isr_load();
  irq_install();

  memcpy((char*)idt.base, (char*)idt_entries, idt.limit);

  asm("lidt [idt]");
}

void x86::idt_set_gate(  unsigned char id,
                         unsigned long base,
                         unsigned short sel,
                         unsigned char flags) {
    idt_entries[id].offset_low = base & 0xFFFF;
    idt_entries[id].offset_high = (base >> 16) & 0xFFFF;

    idt_entries[id].selector = sel;

    idt_entries[id].type = flags;

    idt_entries[id].reserved = 0;
}

void x86::irq_install() {
  irq_remap();

  idt_set_gate(32, (unsigned)irq0, 0x08, 0x8E);
  idt_set_gate(33, (unsigned)irq1, 0x08, 0x8E);
  idt_set_gate(34, (unsigned)irq2, 0x08, 0x8E);
  idt_set_gate(35, (unsigned)irq3, 0x08, 0x8E);
  idt_set_gate(36, (unsigned)irq4, 0x08, 0x8E);
  idt_set_gate(37, (unsigned)irq5, 0x08, 0x8E);
  idt_set_gate(38, (unsigned)irq6, 0x08, 0x8E);
  idt_set_gate(39, (unsigned)irq7, 0x08, 0x8E);
  idt_set_gate(40, (unsigned)irq8, 0x08, 0x8E);
  idt_set_gate(41, (unsigned)irq9, 0x08, 0x8E);
  idt_set_gate(42, (unsigned)irq10, 0x08, 0x8E);
  idt_set_gate(43, (unsigned)irq11, 0x08, 0x8E);
  idt_set_gate(44, (unsigned)irq12, 0x08, 0x8E);
  idt_set_gate(45, (unsigned)irq13, 0x08, 0x8E);
  idt_set_gate(46, (unsigned)irq14, 0x08, 0x8E);
  idt_set_gate(47, (unsigned)irq15, 0x08, 0x8E);
}

void x86::isr_load() {
  idt_set_gate(0, (unsigned)isr0, 0x08, 0x8E);
  idt_set_gate(1, (unsigned)isr1, 0x08, 0x8E);
  idt_set_gate(2, (unsigned)isr2, 0x08, 0x8E);
  idt_set_gate(3, (unsigned)isr3, 0x08, 0x8E);
  idt_set_gate(4, (unsigned)isr4, 0x08, 0x8E);
  idt_set_gate(5, (unsigned)isr5, 0x08, 0x8E);
  idt_set_gate(6, (unsigned)isr6, 0x08, 0x8E);
  idt_set_gate(7, (unsigned)isr7, 0x08, 0x8E);
  idt_set_gate(8, (unsigned)isr8, 0x08, 0x8E);
  idt_set_gate(9, (unsigned)isr9, 0x08, 0x8E);
  idt_set_gate(10, (unsigned)isr10, 0x08, 0x8E);
  idt_set_gate(11, (unsigned)isr11, 0x08, 0x8E);
  idt_set_gate(12, (unsigned)isr12, 0x08, 0x8E);
  idt_set_gate(13, (unsigned)isr13, 0x08, 0x8E);
  idt_set_gate(14, (unsigned)isr14, 0x08, 0x8E);
  idt_set_gate(15, (unsigned)isr15, 0x08, 0x8E);
  idt_set_gate(16, (unsigned)isr16, 0x08, 0x8E);
  idt_set_gate(17, (unsigned)isr17, 0x08, 0x8E);
  idt_set_gate(18, (unsigned)isr18, 0x08, 0x8E);
  idt_set_gate(19, (unsigned)isr19, 0x08, 0x8E);
  idt_set_gate(20, (unsigned)isr20, 0x08, 0x8E);
  idt_set_gate(21, (unsigned)isr21, 0x08, 0x8E);
  idt_set_gate(22, (unsigned)isr22, 0x08, 0x8E);
  idt_set_gate(23, (unsigned)isr23, 0x08, 0x8E);
  idt_set_gate(24, (unsigned)isr24, 0x08, 0x8E);
  idt_set_gate(25, (unsigned)isr25, 0x08, 0x8E);
  idt_set_gate(26, (unsigned)isr26, 0x08, 0x8E);
  idt_set_gate(27, (unsigned)isr27, 0x08, 0x8E);
  idt_set_gate(28, (unsigned)isr28, 0x08, 0x8E);
  idt_set_gate(29, (unsigned)isr29, 0x08, 0x8E);
  idt_set_gate(30, (unsigned)isr30, 0x08, 0x8E);
  idt_set_gate(31, (unsigned)isr31, 0x08, 0x8E);
}

void x86::irq_install_handler(int irq, void (*handler)(struct regs *r)) {
  irq_routines[irq] = handler;
}

void x86::irq_uninstall_handler(int irq) {
  irq_routines[irq] = 0;
}

void x86::irq_remap(void) {
  io.outportb(0x20, 0x11);
  io.outportb(0xA0, 0x11);
  io.outportb(0x21, 0x20);
  io.outportb(0xA1, 0x28);
  io.outportb(0x21, 0x04);
  io.outportb(0xA1, 0x02);
  io.outportb(0x21, 0x01);
  io.outportb(0xA1, 0x01);
  io.outportb(0x21, 0x0);
  io.outportb(0xA1, 0x0);
}

extern "C" void irq_handler(struct regs *r) {
  void (*handler)(struct regs *r);

  handler = irq_routines[r->int_no - 32];
  if(handler) {
    handler(r);
  }

  if(r->int_no >= 40) {
    io.outportb(0xA0, 0x20);
  }

  io.outportb(0x20, 0x20);
}

extern "C" void fault_handler(struct regs *r) {
  if(r->int_no < 32) {
    io.printf("\n");
    if(r->int_no > 18) {
      io.printf(exception_messages[19]);
      io.printf("\nException. System Halted!\n");
    } else {
      io.printf(exception_messages[r->int_no]);
      io.printf("\nException. System Halted!\n");
    }
  }

  while(true);
}
