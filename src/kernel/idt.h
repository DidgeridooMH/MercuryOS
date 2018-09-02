#ifndef IDT_H
#define IDT_H

#define IDT_BASE 0x00000000

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

void idt_load();
void idt_set_gate(  unsigned char id,
                    unsigned long base,
                    unsigned short sel,
                    unsigned char flags);

extern "C" void idt_set();

#endif
