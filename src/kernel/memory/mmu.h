#ifndef PAGING_H
#define PAGING_H

#define PAGE_DIR_ADDRESS    0x1000
#define PAGE_TABLE_ADDRESS  0x400000
#define IDENTITY_PAGE_SIZE  0x100000

namespace MMU {
    unsigned char* map_page(unsigned int page_num, unsigned int virtual_address);
    void unmap_page(unsigned int page_num);
    void paging_load();
    void generate_paging_tables(unsigned int* pageTableAddr, unsigned int vaddr, int size);
    void enable_paging(unsigned int* directory);
    void disable_paging();
};

#endif
