#ifndef PAGING_H
#define PAGING_H

#define PAGE_DIR_ADDRESS     0x2000
#define VIRTUAL_PD_ADDRESS   0xC0001000
#define PAGE_TABLE_ADDRESS   0x400000
#define IDENTITY_PAGE_SIZE   0x100000

#define KERNEL_VIRTUAL_ADDRESS 0xC0002000
#define KERNEL_LINEAR_ADDRESS  0x00100000
#define JUMP_CODE_SIZE         0x9
#define KERNEL_VIRTUAL_JUMP (KERNEL_VIRTUAL_ADDRESS - KERNEL_LINEAR_ADDRESS + JUMP_CODE_SIZE)

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
