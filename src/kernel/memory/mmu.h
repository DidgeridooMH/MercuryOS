#ifndef PAGING_H
#define PAGING_H

#define PAGE_DIR_ADDRESS      0xC0010000
#define PAGE_PHYSICAL_ADDRESS 0x00010000
#define PAGE_TABLE_ADDRESS    0xC0400000

void map_page(void* physical_address, void* virtual_address, unsigned int flags);

void unmap_page(void* virtual_address);

void map_page_pse(void* physical_address, void* virtual_address, unsigned int flags);

void unmap_page_pse(void* virtual_address);

void paging_load();

void generate_paging_tables(unsigned int* pageTableAddr, unsigned int vaddr, int size);

void enable_paging();

void disable_paging();

void* get_next_available_virtual_address();

void* kmalloc(unsigned long n);

void kfree(void* address);

#endif
