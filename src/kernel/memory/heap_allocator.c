#include "heap_allocator.h"
#include "mmu.h"
#include "../../runtime/itoa.h"
#include "../../drivers/io.h"

void initialize_page_heap() {
    for(int i = PAGE_HEAP_ADDRESS; i < 0x400000 + PAGE_HEAP_ADDRESS; i += 0x1000) {
        map_page((unsigned int*)i, (unsigned int*)i, 2);
    }

    struct page_frame* process_ids = (struct page_frame*)PAGE_HEAP_ADDRESS;

    for(int i = 0; (i + 1) * sizeof(struct page_frame) < 0x400000; i++) {
        process_ids[i].slot_free = 1;
    }
}

struct page_frame* create_frame(unsigned int process_id, unsigned int address, unsigned int size) {
    struct page_frame* process_ids = (struct page_frame*)PAGE_HEAP_ADDRESS;

    int page_index = -1;

    for(int i = 0; (i + 1) * sizeof(struct page_frame) < 0x400000 && page_index < 0; i++) {
        if(process_ids[i].slot_free) {
            page_index = i;
        }
    }

    if(page_index < -1) {
        io_printf("Oh no! Out of RAM.");
        return (void*)(0);
    }

    process_ids[page_index].process_id = process_id;
    process_ids[page_index].address = address;
    process_ids[page_index].size = size;
    process_ids[page_index].prev = (void*)(0);
    process_ids[page_index].next = (void*)(0);
    process_ids[page_index].free = 1;
    process_ids[page_index].slot_free = 0;

    return &process_ids[page_index];
}

void* allocate_memory(unsigned int process_id, unsigned int size) {
    struct page_frame* process_ids = (struct page_frame*)PAGE_HEAP_ADDRESS;

    int page_index = -1;

    for(int i = 0; (i + 1) * sizeof(struct page_frame) < 0x400000 && page_index < 0; i++) {
        if(process_ids[i].process_id == process_id
            && process_ids[i].size >= size
            && process_ids[i].free) {
            page_index = i;
        }
    }

    if(page_index < 0) {
        // TODO: this needs to be more robust to account for large memory allocations.
        void* new_frame_address = get_next_available_virtual_address();
        map_page(new_frame_address, new_frame_address, 2);

        if(create_frame(process_id, (unsigned int)new_frame_address, 0x1000) == (void*)(0)) {
            return (void*)(0);
        }

        return allocate_memory(process_id, size);
    }

    if(process_ids[page_index].size == size) {
        process_ids[page_index].free = 0;
        return (void*)process_ids[page_index].address;
    }

    struct page_frame* next = create_frame(
        process_id,
        process_ids[page_index].address + size,
         process_ids[page_index].size - size
    );

    process_ids[page_index].size = size;
    process_ids[page_index].free = 0;
    process_ids[page_index].next = next;
    next->prev = &process_ids[page_index];

    return (void*)process_ids[page_index].address;
}

void deallocate_memory(void* address) {
    struct page_frame* process_ids = (struct page_frame*)PAGE_HEAP_ADDRESS;

    for(int i = 0; (i + 1) * sizeof(struct page_frame) < 0x400000; i++) {
        if(process_ids[i].address == (unsigned int)address) {
            process_ids[i].free = 1;
            merge_contiguous(&process_ids[i]);
            break;
        }
    }
}

void merge_contiguous(struct page_frame* frame) {
    if(frame->prev != (void*)(0) && frame->prev->free) {
        frame->prev->size += frame->size;
        frame->prev->next = frame->next;
        frame->slot_free = 1;
        merge_contiguous(frame->prev);
    } else if (frame->next != (void*)(0) && frame->next->free) {
        frame->size += frame->next->size;
        frame->next = frame->next->next;
        frame->next->slot_free = 1;
        merge_contiguous(frame);
    }
}

void delete_frame(unsigned int process_id) {
    struct page_frame* process_ids = (struct page_frame*)PAGE_HEAP_ADDRESS;

    int frame_id = -1;

    for(int i = 0; (i + 1) * sizeof(struct page_frame) < 0x400000 && frame_id < 0; i++) {
        if(process_ids[i].process_id == process_id) {
            frame_id = i;
        }
    }

    if(frame_id >= 0) {
        struct page_frame* current = &process_ids[frame_id];

        while (current != (void*)(0)) {
            current->slot_free = 1;
            current = current->next;
        }
    }
}
