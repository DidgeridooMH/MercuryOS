#include "heap_allocator.h"
#include "mmu.h"
#include "../../runtime/itoa.h"
#include "../../drivers/io.h"

void Heap::initialize_page_heap() {
    for(int i = PAGE_HEAP_ADDRESS; i < 0x400000 + PAGE_HEAP_ADDRESS; i += 0x1000) {
        MMU::map_page((unsigned int*)i, (unsigned int*)i, 2);
    }

    PageFrame* process_ids = (PageFrame*)PAGE_HEAP_ADDRESS;

    for(int i = 0; (i + 1) * sizeof(PageFrame) < 0x400000; i++) {
        process_ids[i].slot_free = true;
    }
}

Heap::PageFrame* Heap::create_frame(unsigned int process_id, unsigned int address, unsigned int size) {
    PageFrame* process_ids = (PageFrame*)PAGE_HEAP_ADDRESS;

    int page_index = -1;

    for(int i = 0; (i + 1) * sizeof(PageFrame) < 0x400000 && page_index < 0; i++) {
        if(process_ids[i].slot_free) {
            page_index = i;
        }
    }

    if(page_index < -1) {
        Io::printf("Oh no! Out of RAM.");
        return nullptr;
    }

    process_ids[page_index].process_id = process_id;
    process_ids[page_index].address = address;
    process_ids[page_index].size = size;
    process_ids[page_index].prev = nullptr;
    process_ids[page_index].next = nullptr;
    process_ids[page_index].free = true;
    process_ids[page_index].slot_free = false;

    return &process_ids[page_index];
}

void* Heap::allocate_memory(unsigned int process_id, unsigned int size) {
    PageFrame* process_ids = (PageFrame*)PAGE_HEAP_ADDRESS;

    int page_index = -1;

    for(int i = 0; (i + 1) * sizeof(PageFrame) < 0x400000 && page_index < 0; i++) {
        if(process_ids[i].process_id == process_id
            && process_ids[i].size >= size
            && process_ids[i].free) {
            page_index = i;
        }
    }

    if(page_index < 0) {
        // TODO: this needs to be more robust to account for large memory allocations.
        void* new_frame_address = MMU::get_next_available_virtual_address();
        MMU::map_page(new_frame_address, new_frame_address, 2);

        if(create_frame(process_id, (unsigned int)new_frame_address, 0x1000) == nullptr) {
            return nullptr;
        }

        return allocate_memory(process_id, size);
    }

    if(process_ids[page_index].size == size) {
        process_ids[page_index].free = false;
        return (void*)process_ids[page_index].address;
    }

    PageFrame* next = create_frame(
        process_id,
        process_ids[page_index].address + size,
         process_ids[page_index].size - size
    );

    process_ids[page_index].size = size;
    process_ids[page_index].free = false;
    process_ids[page_index].next = next;
    next->prev = &process_ids[page_index];

    return (void*)process_ids[page_index].address;
}

void Heap::deallocate_memory(void* address) {
    PageFrame* process_ids = (PageFrame*)PAGE_HEAP_ADDRESS;

    for(int i = 0; (i + 1) * sizeof(PageFrame) < 0x400000; i++) {
        if(process_ids[i].address == (unsigned int)address) {
            process_ids[i].free = true;
            merge_contiguous(&process_ids[i]);
            break;
        }
    }
}

void Heap::merge_contiguous(PageFrame* frame) {
    if(frame->prev != nullptr && frame->prev->free) {
        frame->prev->size += frame->size;
        frame->prev->next = frame->next;
        frame->slot_free = true;
        merge_contiguous(frame->prev);
    } else if (frame->next != nullptr && frame->next->free) {
        frame->size += frame->next->size;
        frame->next = frame->next->next;
        frame->next->slot_free = true;
        merge_contiguous(frame);
    }
}

void Heap::delete_frame(unsigned int process_id) {
    PageFrame* process_ids = (PageFrame*)PAGE_HEAP_ADDRESS;

    int frame_id = -1;

    for(int i = 0; (i + 1) * sizeof(PageFrame) < 0x400000 && frame_id < 0; i++) {
        if(process_ids[i].process_id == process_id) {
            frame_id = i;
        }
    }

    if(frame_id >= 0) {
        PageFrame* current = &process_ids[frame_id];

        while (current != nullptr) {
            current->slot_free = true;
            current = current->next;
        }
    }
}
