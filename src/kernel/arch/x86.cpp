#include "x86.h"
#include "../../runtime/memory.h"
#include "../../runtime/itoa.h"
#include "../../drivers/vga.h"

struct gdt_entry gdt_entries[GDT_SIZE];
struct gdt_descriptor gdt;

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
