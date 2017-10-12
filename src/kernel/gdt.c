#include "gdt.h"

void gdt_set_entry( int id,
                    unsigned long base,
                    unsigned long limit,
                    unsigned char access,
                    unsigned char flags) {

gdt_entries[id].base_0_15 = base & 0xFFFF;
gdt_entries[id].limit_0_15 = limit & 0xFFFF;
gdt_entries[id].base_24_31 = (base >> 24) & 0xFF;
gdt_entries[id].limit_flags = ((limit >> 16) & 0xF) | (flags & 0xF0);
gdt_entries[id].access = access;
gdt_entries[id].base_16_23 = (base >> 16) & 0xFF;

}

void gdt_load(void) {
    gdt.size = (sizeof(struct gdt_descriptor) * 3) - 1;
    gdt.offset = &gdt_entries;

    gdt_set_entry(0, 0, 0, 0, 0);
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    asm ( "xchg bx, bx"
          "lgdt [1]"
          :
          : "b"(gdt)
        );
  //  gdt_set();
}
