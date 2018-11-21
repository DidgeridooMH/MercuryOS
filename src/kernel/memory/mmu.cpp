#include "mmu.h"
#include "../../drivers/io.h"
#include "../../runtime/itoa.h"
#include "../../runtime/memory.h"

unsigned int directoryAddr;

/*
 * GCC Messes up some calculations when optimizing.
 */
unsigned char* MMU::map_page(unsigned int page_num, unsigned int virtual_address) {
    unsigned int* pageDir = (unsigned int*)PAGE_DIR_ADDRESS;
    unsigned int* logical_address = 0;
    if(page_num > 0) {
        logical_address = (unsigned int*)(PAGE_TABLE_ADDRESS + 0x400000 * page_num);
        disable_paging();
        pageDir[page_num] = (unsigned int)(logical_address) | 1;
        generate_paging_tables(logical_address, virtual_address, 0x400000);
        enable_paging((unsigned int*)PAGE_DIR_ADDRESS);
    } else {
        Io::printf("\nKERNEL_PANIC: Identity page compromised.");
    }
    return (unsigned char*)(0x400000 * page_num);
}

void MMU::unmap_page(unsigned int page_num) {
    unsigned int* pageDir = (unsigned int*)PAGE_DIR_ADDRESS;
    if(page_num > 0) {
        disable_paging();
        pageDir[page_num] = 0;
        enable_paging((unsigned int*)PAGE_DIR_ADDRESS);
    } else {
        Io::printf("\nKERNEL_PANIC: Identity page compromised.");
    }
}

void MMU::paging_load() {
    unsigned int* pageDir = (unsigned int*)PAGE_DIR_ADDRESS;
    pageDir[0] = PAGE_TABLE_ADDRESS | 1;
    generate_paging_tables((unsigned int*)PAGE_TABLE_ADDRESS, 0, 0x400000);
    enable_paging(pageDir);
}

void MMU::generate_paging_tables(unsigned int* pageTableAddr, unsigned int vaddr, int size) {
    vaddr = vaddr & 0xFFFFF000;

    while(size > 0) {
        *pageTableAddr = vaddr | 1;

        vaddr += 0x1000;
        size -= 0x1000;
        pageTableAddr++;
    }
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
