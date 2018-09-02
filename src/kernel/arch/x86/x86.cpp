#include "x86.h"
#include "isr.h"
#include "irq.h"
#include "../../../runtime/memory.h"
#include "../../../runtime/itoa.h"
#include "../../../drivers/io.h"

struct gdt_entry gdt_entries[GDT_SIZE];
struct gdt_descriptor gdt;
struct idt_entry idt_entries[256];
struct idt_descriptor idt;

void gdt_set_entry( int id,
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

void gdt_load() {
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

    gdt_set();
}

void idt_load() {
  idt.limit = sizeof(struct idt_entry) * 0xFF;
  idt.base = IDT_BASE;

  memset((char*)idt_entries, 0, sizeof(struct idt_entry) * 256);

  isr_load();
  irq_install();

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
