#include "../runtime/memory.h"
#include "../drivers/vga.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"

struct idt_entry idt_entries[256];
struct idt_descriptor idt;

void idt_load() {
  idt.limit = sizeof(struct idt_entry) * 0xFF;
  idt.base = IDT_BASE;

  memset((char*)idt_entries, 0, sizeof(struct idt_entry) * 256);

  isr_load();
  irq_install();
  printf("IRQ Installed\n");

  memcpy((char*)idt.base, (char*)idt_entries, idt.limit);

  idt_set();
}

void idt_set_gate(  unsigned char id,
                    unsigned long base,
                    unsigned short sel,
                    unsigned char flags) {
    idt_entries[id].offset_low = base & 0xFFFF;
    idt_entries[id].offset_high = (base >> 16) & 0xFFFF;

    idt_entries[id].selector = sel;

    idt_entries[id].type = flags;

    idt_entries[id].reserved = 0;
}
