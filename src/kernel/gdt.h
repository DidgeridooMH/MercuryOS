#ifndef GDT_H
#define GDT_H

struct gdt_descriptor {
  unsigned short  size;
  unsigned int    offset;
} __attribute__((packed));

struct gdt_entry {
  unsigned short  limit_0_15;
  unsigned short  base_0_15;
  unsigned char   base_16_23;
  unsigned char   access;
  unsigned char   limit_flags;
  unsigned char   base_24_31;
} __attribute__((packed));

struct gdt_entry gdt_entries[3];
struct gdt_descriptor gdt;

void gdt_load(void);
void gdt_set_entry( int id,
                    unsigned long base,
                    unsigned long limit,
                    unsigned char access,
                    unsigned char flags);

void gdt_set();


#endif
