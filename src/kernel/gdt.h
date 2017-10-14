#ifndef GDT_H
#define GDT_H

struct gdt_entry {
  unsigned short  limit;
  unsigned short  base_low;
  unsigned char   base_middle;
  unsigned char   access;
  unsigned char   granularity;
  unsigned char   base_high;
} __attribute__((packed));

struct gdt_descriptor {
  unsigned short    size;
  unsigned int    offset;
} __attribute__((packed));

struct gdt_entry gdt_entries[5];
struct gdt_descriptor gdt;

void gdt_load(void);
void gdt_set_entry( int id,
                    unsigned long base,
                    unsigned long limit,
                    unsigned char access,
                    unsigned char flags);

void gdt_set();


#endif
