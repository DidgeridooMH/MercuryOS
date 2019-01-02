#ifndef HEAP_ALLOCATOR_H
#define HEAP_ALLOCATOR_H

#define PAGE_HEAP_ADDRESS 0x800000

namespace Heap {


    struct PageFrame {
        unsigned int process_id;
        unsigned int address;
        unsigned int size;
        PageFrame*   prev;
        PageFrame*   next;
        bool         free;
        bool         slot_free;
    } __attribute__((packed));

    void initialize_page_heap();

    PageFrame* create_frame(unsigned int process_id, unsigned int address, unsigned int size);

    void delete_frame(unsigned int process_id);

    void* allocate_memory(unsigned int process_id, unsigned int size);

    void deallocate_memory(void* address);

    void merge_contiguous(PageFrame* frame);
}

#endif
