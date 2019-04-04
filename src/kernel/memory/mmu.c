#include "mmu.h"
#include "../../drivers/io.h"
#include "../../runtime/itoa.h"
#include "../../runtime/memory.h"
#include "../arch/x86/x86.h"

unsigned int _directory_address;

unsigned int _virtual_address;

int _paging_enabled = 0;

void paging_load() {
    unsigned int* page_directory = (unsigned int*)PAGE_DIR_ADDRESS;

    for(int i = 0; i < 0x400; i++) {
        // Each table is 1024 entries * 4 bytes. The 2 marks it R/W.
        page_directory[i] = 2;
    }

    map_page_pse(0x0, 0xC0000000, 2);
    map_page_pse(0x400000, 0xC0400000, 2);
    map_page_pse(0x800000, 0xC0800000, 2);
    map_page_pse(0x800000, 0xC0C00000, 2);
    map_page_pse(0x800000, 0xC1000000, 2);

    enable_paging();
}

void map_page_pse(void* physical_address, void* virtual_address, unsigned int flags) {
    unsigned int page_dir_index = (unsigned int) virtual_address / 0x400000;

    unsigned int* page_dir = (unsigned int*)(PAGE_DIR_ADDRESS);

    if(page_dir[page_dir_index] & 1) {
        io_printf("Paging error: pse index already assigned...we should do something about that\n");
    }
    page_dir[page_dir_index] = (unsigned int)physical_address | 0x81 | flags;
    asm("mov eax, _virtual_address \n \
         invlpg byte ptr [eax]");
}

void unmap_page_pse(void* virtual_address) {
    unsigned int page_dir_index = (unsigned int)virtual_address >> 22;
    unsigned int* page_dir = (unsigned int*)PAGE_DIR_ADDRESS;
    page_dir[page_dir_index] = 0x2;
}

void map_page(void* physical_address, void* virtual_address, unsigned int flags) {
    unsigned int page_dir_index = (unsigned int)virtual_address / 0x400000;
    unsigned int page_table_index = ((unsigned int)virtual_address % 0x400000) / 0x1000;

    unsigned int* page_dir = (unsigned int*)(PAGE_DIR_ADDRESS);

    // Check if the page directory entry is present. If not, create a new table.
    if(!(page_dir[page_dir_index] & 1)) {
        unsigned int* table_ptr = ((unsigned int*)PAGE_TABLE_ADDRESS) + 0x400 * page_dir_index;
        for(int i = 0; i < 0x400; i++) {
            // Mark not present
            table_ptr[i] = 0;
        }
        page_dir[page_dir_index] = (unsigned int)table_ptr + 3;
    }

    unsigned int* page_table = (unsigned int*)(PAGE_TABLE_ADDRESS + 0x1000 * page_dir_index);

    if(page_table[page_table_index] & 1) {
        io_printf("Paging error: page table already assigned...we should do something about that\n");
        return;
    }

    page_table[page_table_index] = ((unsigned int) physical_address) | (flags & 0xFFF) | 0x01;

    _virtual_address = (unsigned int)virtual_address;
    asm("mov eax, _virtual_address \n \
         invlpg byte ptr [eax]");
}

void unmap_page(void* virtual_address) {
    unsigned int page_dir_index = (unsigned int)virtual_address / 0x400000;
    unsigned int page_table_index = ((unsigned int)virtual_address % 0x400000) / 0x1000;
    unsigned int* page_table = ((unsigned int*)PAGE_TABLE_ADDRESS) + 0x400 * page_dir_index;
    page_table[page_table_index] = 0;
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

void* get_next_available_virtual_address() {
    for(int i = 3; i < 1024; i++) {
        unsigned int* table_ptr = ((unsigned int*)PAGE_TABLE_ADDRESS) + 0x400 * i;
        for(int j = 0; j < 1024; j++) {
            if(!(table_ptr[j] & 1)) {
                return (void*)(0x400000 * i + j * 0x1000);
            }
        }
    }

    return (void*)(0);
}

// void* kmalloc(unsigned long n) {
//     if(n > 0x1000) {
//         io_printf("WARNING: allocating more than a page of memory is not implemented");
//         return (void*)(0);
//     }
//
//     return allocate_memory(0, n);
// }
