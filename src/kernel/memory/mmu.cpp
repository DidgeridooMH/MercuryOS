#include "mmu.h"
#include "../../drivers/io.h"
#include "../../runtime/itoa.h"
#include "../../runtime/memory.h"

unsigned int directoryAddr;

void MMU::paging_load() {
    unsigned int* pageDir = (unsigned int*)PAGE_DIR_ADDRESS;

    for(int i = 0; i < 1024; i++) {
        if(i < 4) {
            pageDir[i] = (PAGE_TABLE_ADDRESS + 1024 * i) | 1;
        } else {
            pageDir[i] = 0x2;
        }
    }

    identity_paging((unsigned int*)PAGE_TABLE_ADDRESS, 0, 0x400000);

    enable_paging(pageDir);
}

// lowest 2MB will be identity paged.
void MMU::identity_paging(unsigned int* pageTableAddr, unsigned int vaddr, int size) {
    vaddr = vaddr & 0xFFFFF000;

    while(size > 0) {
        *pageTableAddr = vaddr | 1;

        /* Verbose */
        char entry[8];
        memset(entry, 0, 8);
        char address[8];
        memset(address, 0, 8);
        itoa(entry, vaddr | 1, 16);
        itoa(address, (unsigned)pageTableAddr, 16);
        Io::printf("Wrote entry {0x");
        Io::printf(entry);
        Io::printf("} at {0x");
        Io::printf(address);
        Io::printf("}\n");

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
