#include "paging.h"

unsigned int _directory_address;

void paging_load() {
    unsigned int *page_directory = (unsigned int *)PAGE_DIR_ADDRESS;

    for (int i = 0; i < 0x400; i++) {
        // Each table is 1024 entries * 4 bytes. The 2 marks it R/W.
        page_directory[i] = 2;
    }

    for (int i = 0; i <= 0x1400000; i += 0x400000) {
        map_page_pse(i, 0xC0000000 + i, 2);
    }

    enable_paging();
}

void map_page_pse(void *physical_address, void *virtual_address,
                  unsigned int flags) {
    unsigned int page_dir_index = (unsigned int)virtual_address / 0x400000;

    unsigned int *page_dir = (unsigned int *)(PAGE_DIR_ADDRESS);

    if (page_dir[page_dir_index] & 1) {
        io_printf(
            "Paging error: pse index already assigned...we should do something "
            "about that\n");
    }
    page_dir[page_dir_index] = (unsigned int)physical_address | 0x81 | flags;
    asm("mov eax, 0 \n \
         invlpg byte ptr [eax]");
}

void unmap_page_pse(void *virtual_address) {
    unsigned int page_dir_index = (unsigned int)virtual_address >> 22;
    unsigned int *page_dir = (unsigned int *)PAGE_DIR_ADDRESS;
    page_dir[page_dir_index] = 0x2;

    asm("mov eax, 0 \n \
         invlpg byte ptr [eax]");
}

void map_page(void *physical_address, void *virtual_address,
              unsigned int flags) {
    unsigned int page_dir_index = (unsigned int)virtual_address / 0x400000;
    unsigned int page_table_index =
        ((unsigned int)virtual_address >> 12) & 0x3FF;

    unsigned int *page_dir = (unsigned int *)(PAGE_DIR_ADDRESS);
    unsigned int *page_table =
        (unsigned int *)(PAGE_TABLE_ADDRESS + 0x1000 * page_dir_index);
    unsigned int *page_table_phys =
        (unsigned int *)(0x400000 + 0x1000 * page_dir_index);

    // Check if the page directory entry is present. If not, create a new table.
    if (!(page_dir[page_dir_index] & 1)) {
        for (int i = 0; i < 0x400; i++) {
            // Mark not present
            page_table[i] = 2;
        }
        page_dir[page_dir_index] = (unsigned int)page_table_phys | 3;
    }

    if (page_table[page_table_index] & 1) {
        io_printf(
            "Paging error: page table already assigned...we should do "
            "something about that\n");
        return;
    }

    page_table[page_table_index] =
        ((unsigned int)physical_address) | (flags & 0xFFF) | 0x01;

    asm("mov eax, 0 \n \
         invlpg byte ptr [eax]");
}

void unmap_page(void *virtual_address) {
    unsigned int page_dir_index = (unsigned int)virtual_address / 0x400000;
    unsigned int page_table_index =
        ((unsigned int)virtual_address >> 12) & 0x3FF;
    unsigned int *page_table =
        (unsigned int *)(PAGE_TABLE_ADDRESS + 0x1000 * page_dir_index);
    page_table[page_table_index] = 0;

    asm("mov eax, 0 \n \
         invlpg byte ptr [eax]");
}

void *get_physical_address(void *virtual_addr) {
    unsigned long pdindex = (unsigned long)virtual_addr >> 22;
    unsigned long ptindex = (unsigned long)virtual_addr >> 12 & 0x03FF;

    unsigned long *pd = (unsigned long *)PAGE_DIR_ADDRESS;
    unsigned long *pt =
        ((unsigned long *)PAGE_TABLE_ADDRESS) + (0x400 * pdindex);

    return (void *)((pt[ptindex] & ~0xFFF) +
                    ((unsigned long)virtual_addr & 0xFFF));
}

void enable_paging() {
    _directory_address = PAGE_PHYSICAL_ADDRESS;
    asm("mov eax, _directory_address     \n \
         mov cr3, eax               \n \
         mov eax, cr0               \n \
         or eax, 0x80000001         \n \
         mov cr0, eax               \n");
}

void disable_paging() {
    asm("mov eax, 0x60000011        \n \
         mov cr0, eax               \n \
         mov eax, 0                 \n \
         mov cr3, eax               \n ");
}