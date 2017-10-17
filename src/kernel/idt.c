#include "../runtime/memory.h"
#include "idt.h"

void idt_load() {
  idt.limit = (sizeof(struct idt_entry) * 256) - 1;
  idt.base = &idt_entries;

  memset(&idt_entries, 0, sizeof(struct idt_entry) * 256);

  isr_load();

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
