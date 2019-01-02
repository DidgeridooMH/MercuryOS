#ifndef HEAP_ALLOCATOR_H
#define HEAP_ALLOCATOR_H

#define PAGE_HEAP_ADDRESS 0x800000

namespace Heap {

    union FrameFlags {
        bool flag0;
        bool flag1;
        bool flag2;
        bool flag3;
        bool flag4;
        bool flag5;
        bool free;
        bool slot_free;
    } __attribute__((packed));

    struct PageFrame {
        unsigned int process_id;
        unsigned int address;
        unsigned int size;
        unsigned char _reserved;
        unsigned short next;
        FrameFlags   flags;
    } __attribute__((packed));

    void initialize_page_heap();

    void create_frame(unsigned int process_id, unsigned int address, unsigned int size);

    void delete_frame(unsigned int process_id);

    void* allocate_memory(unsigned int process_id, unsigned int size);

    void deallocate_memory(void* address);
}

#endif
