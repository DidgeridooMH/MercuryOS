#include "mmu.h"
#include "../../drivers/io.h"
#include "../../runtime/itoa.h"
#include "../../runtime/memory.h"

// TODO: Decide on a code style.

unsigned int directoryAddr;
unsigned int _virtual_address;

void MMU::paging_load() {
    unsigned int* pageDir = (unsigned int*)PAGE_DIR_ADDRESS;
    // pageDir[0] = PAGE_TABLE_ADDRESS | 1;
    // generate_paging_tables((unsigned int*)PAGE_TABLE_ADDRESS, 0, 0x400000);

    for(int i = 0; i < 1024; i++) {
        // Each table is 1024 entries * 4 bytes. The 2 marks it present and R/W.
        pageDir[i] = PAGE_TABLE_ADDRESS + 0x1000 * i + 2;
    }

    for(int i = 0; i < 0x800000; i += 0x1000) {
        map_page((unsigned int*)i, (unsigned int*)i, 2);
    }

    enable_paging(pageDir);
}

void MMU::map_page(void* physical_address, void* virtual_address, unsigned int flags) {
    unsigned int page_dir_index = (unsigned int)virtual_address / 0x400000;
    unsigned int page_table_index = ((unsigned int)virtual_address % 0x400000) / 0x1000;

    unsigned int* page_dir = (unsigned int*)PAGE_DIR_ADDRESS;

    // Check if the page directory entry is present. If not, create a new table.
    if(!(page_dir[page_dir_index] & 1)) {
        unsigned int* table_ptr = ((unsigned int*)PAGE_TABLE_ADDRESS) + 0x400 * page_dir_index;
        for(int i = 0; i < 1024; i++) {
            // Mark not present
            table_ptr[i] = 0;
        }
        page_dir[page_dir_index] = (unsigned int)table_ptr + 3;
    }

    unsigned int* page_table = (unsigned int*)(PAGE_TABLE_ADDRESS + 0x1000 * page_dir_index);

    if(page_table[page_table_index] & 1) {
        Io::printf("OH SHIT > THERES ALREADY A PAGE TABLE PRESENT FOR THAT VIRTUAL ADDRESS!!!\n");
        return;
    }

    page_table[page_table_index] = ((unsigned int) physical_address) | (flags & 0xFFF) | 0x01;
    _virtual_address = (unsigned int)virtual_address;
    asm("mov eax, _virtual_address \n \
         invlpg byte ptr [eax]");
}

void MMU::unmap_page(void* virtual_address) {
    unsigned int page_dir_index = (unsigned int)virtual_address >> 22;
    unsigned int page_table_index = (unsigned int) virtual_address >> 12;
    unsigned int* page_table = ((unsigned int*)PAGE_TABLE_ADDRESS) + 0x400 * page_dir_index;
    page_table[page_table_index] = 0;
}

void MMU::enable_paging(unsigned int* directory) {
    directoryAddr = (unsigned int)directory;
    asm("mov eax, directoryAddr     \n \
         mov cr3, eax               \n \
         mov eax, cr0               \n \
         or eax, 0x80000001         \n \
         mov cr0, eax               \n");
}

void MMU::disable_paging() {
    asm("mov eax, 0x60000011        \n \
         mov cr0, eax               \n \
         mov eax, 0                 \n \
         mov cr3, eax               \n ");
}
