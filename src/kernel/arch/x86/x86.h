#ifndef GDT_H
#define GDT_H

#define GDT_SIZE 0xFF
#define GDT_BASE 0x00000800
#define IDT_BASE 0x00000000

#define GDT_VIRTUAL_BASE 0xC0000800
#define IDT_VIRTUAL_BASE 0xC0000000

#include "system.h"

struct gdt_entry {
  unsigned short  limit;
  unsigned short  base_low;
  unsigned char   base_middle;
  unsigned char   access;
  unsigned char   granularity;
  unsigned char   base_high;
} __attribute__((packed));

struct gdt_descriptor {
  unsigned short  size;
  unsigned int    offset;
} __attribute__((packed));


struct idt_entry {
  unsigned short  offset_low;
  unsigned short  selector;
  unsigned char   reserved;
  unsigned char   type;
  unsigned short  offset_high;
} __attribute__((packed));

struct idt_descriptor {
  unsigned short  limit;
  unsigned int    base;
} __attribute__((packed));

struct gdt_entry gdt_entries[GDT_SIZE];
struct idt_entry idt_entries[256];

void gdt_load(unsigned int address);
void gdt_set_entry(int id, unsigned long base, unsigned long limit,
                    unsigned char access, unsigned char flags);

void idt_set();

void idt_load();

void idt_set_gate(unsigned char id, unsigned long base, unsigned short sel, unsigned char flags);

void isr_load();

void irq_install_handler(int irq, void (*handler)(struct regs *r));

void irq_uninstall_handler(int irq);

void irq_remap(void);

void irq_install();

#endif
