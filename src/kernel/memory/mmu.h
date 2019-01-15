#ifndef PAGING_H
#define PAGING_H

#define PAGE_DIR_ADDRESS    0x1000
#define PAGE_TABLE_ADDRESS  0x400000
#define IDENTITY_PAGE_SIZE  0x100000

void map_page(void* physical_address, void* virtual_address, unsigned int flags);

void unmap_page(void* virtual_address);

void paging_load();

void generate_paging_tables(unsigned int* pageTableAddr, unsigned int vaddr, int size);

void enable_paging();

void disable_paging();

void* get_next_available_virtual_address();

void* kmalloc(unsigned long n);

void kfree(void* address);

#endif
