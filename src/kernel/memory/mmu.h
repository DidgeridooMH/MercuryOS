#ifndef PAGING_H
#define PAGING_H

#define PAGE_DIR_ADDRESS 0xC0010000
#define PAGE_PHYSICAL_ADDRESS 0x00010000
#define PAGE_TABLE_ADDRESS 0xC0400000
#define FRAME_MAP_ADDRESS 0xC1400000

// Currently being limited to 4gb. However, memory should be detected and used.
#define TOTAL_MEMORY_PAGES 0x100000

// These are frames that always allocate and not available for use.
#define PREFILL_FRAMES 0x400
#define FRAME_SIZE 0x1000
#define INFO_SLOTS \
    ((FRAME_SIZE - sizeof(struct frame_header)) / sizeof(struct allocate_info))

#define FRAME_HEAD_ADDRESS (FRAME_MAP_ADDRESS * TOTAL_MEMORY_PAGES)

#define NULL 0x0
#define size_t unsigned int

void setup_frame_alloc();

void *allocate_frame();

void deallocate_frame(void *address);

void map_page(void *physical_address, void *virtual_address,
              unsigned int flags);

void unmap_page(void *virtual_address);

void map_page_pse(void *physical_address, void *virtual_address,
                  unsigned int flags);

void unmap_page_pse(void *virtual_address);

void paging_load();

void generate_paging_tables(unsigned int *pageTableAddr, unsigned int vaddr,
                            int size);

void enable_paging();

void disable_paging();

void *get_next_available_virtual_address();

void *kmalloc(size_t size);

void kfree(void *address);

#endif
