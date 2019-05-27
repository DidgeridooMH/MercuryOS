#include "frame.h"
#include "mmu.h"
#include "paging.h"

void setup_frame_alloc() {
    unsigned char *frames_ptr = (void *)FRAME_MAP_ADDRESS;

    // 4mb -> 4194304bytes / 1024 = 4096kb / 4 = 1024
    for (int i = 0; i < PREFILL_FRAMES; i++) {
        frames_ptr[i] = 1;
    }

    for (int i = PREFILL_FRAMES; i < TOTAL_MEMORY_PAGES; i++) {
        frames_ptr[i] = 0;
    }
}

void *frame_to_address(unsigned int frame) {
    return (void *)(frame * FRAME_SIZE + KERNEL_HEAP_ADDRESS);
}

unsigned int address_to_frame(void *address) {
    return (unsigned int)(address - KERNEL_HEAP_ADDRESS) / FRAME_SIZE;
}

void *get_next_available_frame() {
    unsigned int frame = 0;
    unsigned char *frames_ptr = (void *)FRAME_MAP_ADDRESS;

    for (int i = 0; i < TOTAL_MEMORY_PAGES && !frame; i++) {
        if (!frames_ptr[i]) {
            frame = i;
        }
    }

    return frame_to_address(frame);
}

void *allocate_frame() {
    /* TODO: Should add the newly established frame to the context */
    void *frame = get_next_available_frame();

    unsigned char *frames_ptr = FRAME_MAP_ADDRESS;
    frames_ptr[address_to_frame(frame)] = 1;
    return frame;
}

void deallocate_frame(void *address) {
    /* TODO: Should also check if frame is in context. Otherwise disallow */

    /* TODO: Should delete from context. */

    unsigned char *frames_ptr = FRAME_MAP_ADDRESS;
    frames_ptr[address_to_frame(address)] = 0;
}

void *get_next_available_virtual_frame(unsigned int count) {
    unsigned int cur_count = 0;
    void *address = NULL;
    for (int i = 3; i < 1024; i++) {
        unsigned int *table_ptr =
            (unsigned int *)(PAGE_TABLE_ADDRESS + 0x1000 * i);
        for (int j = 0; j < 1024; j++) {
            if (!(table_ptr[j] & 1)) {
                if (cur_count == 0) {
                    address = (void *)(0x400000 * i + j * 0x1000);
                }
                cur_count++;
                if (cur_count == count) {
                    return address;
                }
            } else {
                address = NULL;
                cur_count = 0;
            }
        }
    }

    return (void *)(0);
}

void *allocate_vframe() {
    void *frame = allocate_frame();
    void *vframe = get_next_available_virtual_frame(1);

    map_page(frame, vframe, 2);
    return vframe;
}

void *allocate_contiguous_vframe(unsigned int size) {
    unsigned int num_of_frames = (size / FRAME_SIZE);
    if (size % FRAME_SIZE) {
        num_of_frames++;
    }
    void *vframe = get_next_available_virtual_frame(num_of_frames);

    for (int i = 0; i < num_of_frames; i++) {
        map_page(allocate_frame(), vframe + FRAME_SIZE * i, 2);
    }

    return vframe;
}

void deallocate_vframe(void *vframe) {
    deallocate_frame(get_physical_address(vframe));
    unmap_page(vframe);
}

void deallocate_contiguous_vframe(void *vframe, unsigned int size) {
    unsigned int num_of_frames = (size / FRAME_SIZE);
    if (size % FRAME_SIZE) {
        num_of_frames++;
    }

    for (int i = 0; i < num_of_frames; i++) {
        deallocate_frame(get_physical_address(vframe + FRAME_SIZE * i));
        unmap_page(vframe + FRAME_SIZE * i);
    }
}

struct frame_header *create_kernel_frame(unsigned int size) {
    unsigned int normalized_size = (size / FRAME_SIZE);
    if (size % FRAME_SIZE) {
        normalized_size++;
    }
    normalized_size *= FRAME_SIZE;

    struct frame_header *next_free = get_kernel_context()->frames;
    while (next_free->next != NULL) {
        next_free = next_free->next;
    }

    struct frame_info *info_frame = allocate_vframe();
    void *data_frame;
    if (size < FRAME_SIZE) {
        data_frame = allocate_vframe();
    } else {
        data_frame = allocate_contiguous_vframe(size);
    }

    /* Frame->used = -1 denotes end of structure */
    info_frame->header.frame_addr = data_frame;
    info_frame->header.alloc_addr = &info_frame->info;
    info_frame->header.next = NULL;
    info_frame->header.prev = next_free;

    (info_frame->info[0]).address = data_frame;
    (info_frame->info[0]).size = normalized_size;
    (info_frame->info[0]).used = 0;
    (info_frame->info[0]).junk = 0;
    (info_frame->info[1]).used = -1;

    next_free->next = &info_frame->header;

    return &info_frame->header;
}

struct allocate_info *find_next_junk(struct allocate_info *head) {
    while (head->used != -1 && head->junk == 0) {
        head++;
    }
    return head;
}

int get_available_allocation(struct frame_header *header, unsigned int size) {
    struct allocate_info *alloc_info = header->alloc_addr;
    struct allocate_info *available = NULL;

    if (alloc_info == NULL) {
        return NULL;
    }

    while (available == NULL && alloc_info->used != -1) {
        if (alloc_info->size >= size && alloc_info->used == 0) {
            available = alloc_info;
        }
        alloc_info++;
    }
    return available;
}