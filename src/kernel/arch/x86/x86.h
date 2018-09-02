#ifndef GDT_H
#define GDT_H

#define GDT_SIZE 0xFF
#define GDT_BASE 0x00000800
#define IDT_BASE 0x00000000

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

void gdt_load();
void gdt_set_entry( int id,
                    unsigned long base,
                    unsigned long limit,
                    unsigned char access,
                    unsigned char flags);

extern "C" void gdt_set();

void idt_load();
void idt_set_gate(  unsigned char id,
                    unsigned long base,
                    unsigned short sel,
                    unsigned char flags);

extern "C" void idt_set();



#endif
