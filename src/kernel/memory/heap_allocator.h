#ifndef HEAP_ALLOCATOR_H
#define HEAP_ALLOCATOR_H

#define PAGE_HEAP_ADDRESS 0x800000

struct page_frame {
    unsigned int process_id;
    unsigned int address;
    unsigned int size;
    struct page_frame*   prev;
    struct page_frame*   next;
    unsigned char free;
    unsigned char slot_free;
} __attribute__((packed));

void initialize_page_heap();

struct page_frame* create_frame(unsigned int process_id, unsigned int address, unsigned int size);

void delete_frame(unsigned int process_id);

void* allocate_memory(unsigned int process_id, unsigned int size);

void deallocate_memory(void* address);

void merge_contiguous(struct page_frame* frame);

#endif
