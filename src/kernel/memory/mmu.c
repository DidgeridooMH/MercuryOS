#include "mmu.h"
#include "../../drivers/io.h"
#include "../../runtime/itoa.h"
#include "../../runtime/memory.h"
#include "../arch/x86/x86.h"
#include "../kernel.h"

unsigned int _directory_address;

unsigned int _virtual_address;

int _paging_enabled = 0;

static unsigned int _kernel_virtual_jump = KERNEL_VIRTUAL_JUMP;

void paging_load() {
    unsigned int* pageDir = (unsigned int*)PAGE_DIR_ADDRESS;

    for(int i = 0; i < 0x400; i++) {
        // Each table is 1024 entries * 4 bytes. The 2 marks it present and R/W.
        pageDir[i] = PAGE_TABLE_ADDRESS + 0x1000 * i + 2;
    }

    for(int i = 0x3000; i < 0x800000; i += 0x1000) {
        map_page((unsigned int*)i, (unsigned int*)i, 2);
    }

    // Map page directory
    unsigned int* pd_virtual_address = (unsigned int*)VIRTUAL_PD_ADDRESS;
    map_page(pageDir, pd_virtual_address, 2);

    // Temporarily identity map the GDT/IDT
    map_page(0, 0, 2);

    enable_paging();

    _paging_enabled = 1;

    // Remap and reload GDT/IDT and unmap temp
    map_page((unsigned int*)IDT_BASE, (unsigned int*)0xC0000000, 2);
    gdt_load(GDT_VIRTUAL_BASE);
    idt_load(IDT_VIRTUAL_BASE);
    unmap_page((unsigned int*)IDT_BASE);

    map_page((unsigned int*)0x100000, (unsigned int*)0xC0002000, 2);
    map_page((unsigned int*)0x101000, (unsigned int*)0xC0003000, 2);
    map_page((unsigned int*)0x102000, (unsigned int*)0xC0004000, 2);
    asm("call next      \n \
         next: pop eax  \n \
         add eax, _kernel_virtual_jump \n \
         jmp eax");
    unmap_page((unsigned int*)0x100000);
    kmain();
}

void map_page(void* physical_address, void* virtual_address, unsigned int flags) {
    unsigned int page_dir_index = (unsigned int)virtual_address / 0x400000;
    unsigned int page_table_index = ((unsigned int)virtual_address % 0x400000) / 0x1000;

    unsigned int* page_dir = (unsigned int*)(_paging_enabled ? VIRTUAL_PD_ADDRESS : PAGE_DIR_ADDRESS);

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
    _directory_address = (unsigned int)PAGE_DIR_ADDRESS;
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
