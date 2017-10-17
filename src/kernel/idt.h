#ifndef IDT_H
#define IDT_H

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

struct idt_entry idt_entries[256];
struct idt_descriptor idt;

void idt_load();
void idt_set_gate(  unsigned char id,
                    unsigned long base,
                    unsigned short sel,
                    unsigned char flags);

void idt_set();

#endif
