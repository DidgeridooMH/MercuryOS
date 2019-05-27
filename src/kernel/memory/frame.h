#ifndef FRAME_H
#define FRAME_H

#define FRAME_MAP_ADDRESS 0xC1400000

#define KERNEL_HEAP_ADDRESS 0x1800000

#define FRAME_HEAD_ADDRESS (FRAME_MAP_ADDRESS * TOTAL_MEMORY_PAGES)

// Currently being limited to 4gb. However, memory should be detected and used.
#define TOTAL_MEMORY_PAGES 0x100000

// These are frames that always allocate and not available for use.
#define PREFILL_FRAMES 0x400
#define FRAME_SIZE 0x1000
#define INFO_SLOTS \
    ((FRAME_SIZE - sizeof(struct frame_header)) / sizeof(struct allocate_info))

#define NULL 0x0

struct frame_header {
    void *frame_addr;
    struct allocate_info *alloc_addr;
    struct frame_header *next;
    struct frame_header *prev;
};

struct allocate_info {
    void *address;
    unsigned int size;
    char used;
    char junk;
};

struct frame_info {
    struct frame_header header;
    struct allocate_info info[INFO_SLOTS];
};

void setup_frame_alloc();

void *allocate_frame();

void deallocate_frame(void *address);

void *get_next_available_virtual_address();

void *get_next_available_frame();

#endif