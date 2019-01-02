#include "heap_allocator.h"
#include "mmu.h"
#include "../../runtime/itoa.h"
#include "../../drivers/io.h"

void Heap::initialize_page_heap() {
    for(int i = PAGE_HEAP_ADDRESS; i < 0x400000 + PAGE_HEAP_ADDRESS; i += 0x1000) {
        MMU::map_page((unsigned int*)i, (unsigned int*)i, 2);
    }

    PageFrame* process_ids = (PageFrame*)PAGE_HEAP_ADDRESS;

    for(int i = 0; i * sizeof(PageFrame) < 0x400000; i++) {
        process_ids[i].flags.slot_free = true;
    }
}

void Heap::create_frame(unsigned int process_id, unsigned int address, unsigned int size) {
    PageFrame* process_ids = (PageFrame*)PAGE_HEAP_ADDRESS;

    int page_index = -1;

    for(int i = 0; i * sizeof(PageFrame) < 0x400000 && page_index < 0; i++) {
        if(process_ids[i].flags.slot_free) {
            page_index = i;
        }
    }

    process_ids[page_index].process_id = process_id;
    process_ids[page_index].address = address;
    process_ids[page_index].size = size;
    process_ids[page_index].flags.free = true;
    process_ids[page_index].flags.slot_free = false;
}

void* Heap::allocate_memory(unsigned int process_id, unsigned int size) {
    PageFrame* process_ids = (PageFrame*)PAGE_HEAP_ADDRESS;

    int page_index = -1;

    for(int i = 0; i * sizeof(PageFrame) < 0x400000 && page_index < 0; i++) {
        if(process_ids[i].process_id == process_id && process_ids[i].size >= size) {
            page_index = i;
        }
    }

    if(page_index < 0) {
        return nullptr;
    }

    if(process_ids[page_index].size == size) {
        process_ids[page_index].flags.free = false;
        return (void*)process_ids[page_index].address;
    }

    create_frame(process_id, process_ids[page_index].address + size, process_ids[page_index].size - size);

    process_ids[page_index].size = size;
    process_ids[page_index].flags.free = false;

    return (void*)process_ids[page_index].address;
}

void Heap::deallocate_memory(void* address) {
    PageFrame* process_ids = (PageFrame*)PAGE_HEAP_ADDRESS;

    for(int i = 0; i * sizeof(PageFrame) < 0x400000; i++) {
        if(process_ids[i].address == (unsigned int)address) {
            process_ids[i].flags.free = true;
            break;
            // TODO: Merge blocks after deallocation
        }
    }
}

void Heap::delete_frame(unsigned int process_id) {
    PageFrame* process_ids = (PageFrame*)PAGE_HEAP_ADDRESS;

    for(int i = 0; i * sizeof(PageFrame) < 0x400000; i++) {
        if(process_ids[i].process_id == process_id) {
            process_ids[i].flags.slot_free = true;
        }
    }
}
