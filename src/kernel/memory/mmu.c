#include "mmu.h"
#include "../../drivers/io.h"
#include "../../runtime/itoa.h"
#include "../../runtime/memory.h"
#include "../arch/x86/x86.h"

unsigned int _directory_address;

struct allocate_info {
    void *address;
    size_t size;
    char used;
    char junk;
};

struct frame_header {
    void *frame_addr;
    struct allocate_info *alloc_addr;
    struct frame_header *next;
    struct frame_header *prev;
};

struct frame_info {
    struct frame_header header;
    struct allocate_info info[INFO_SLOTS];
};

struct memory_context {
    unsigned int id;
    struct frame_header *frames;
};

static struct memory_context kernel_context;
static struct frame_header   kernel_frame_head;

void memory_init() {
    setup_frame_alloc();

    kernel_context.id = 0;
    kernel_context.frames = &kernel_frame_head;

    kernel_frame_head.frame_addr = NULL;
    kernel_frame_head.alloc_addr = NULL;
    kernel_frame_head.next = NULL;
    kernel_frame_head.prev = NULL;
}

void setup_frame_alloc() {
    unsigned char *frames_ptr = (void *)FRAME_MAP_ADDRESS;

    // 4mb -> 4194304bytes / 1024 = 4096kb / 4 = 1024
    for(int i = 0; i < PREFILL_FRAMES; i++) {
        frames_ptr[i] = 1;
    }

    for(int i = PREFILL_FRAMES; i < TOTAL_MEMORY_PAGES; i++) {
        frames_ptr[i] = 0;
    }
}

void *frame_to_address(unsigned int frame) {
    return (void *)(frame * FRAME_SIZE);
}

unsigned int address_to_frame(void *address) {
    return (unsigned int)(address) / FRAME_SIZE;
}

void *get_next_available_frame() {
    unsigned int frame = 0;
    unsigned char *frames_ptr = (void *)FRAME_MAP_ADDRESS;

    for(int i = 0; i < TOTAL_MEMORY_PAGES && !frame; i++) {
        if(!frames_ptr[i]) {
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

void paging_load() {
    unsigned int* page_directory = (unsigned int*)PAGE_DIR_ADDRESS;

    for(int i = 0; i < 0x400; i++) {
        // Each table is 1024 entries * 4 bytes. The 2 marks it R/W.
        page_directory[i] = 2;
    }

    for(int i = 0; i <= 0x1400000; i += 0x400000) {
        map_page_pse(i, 0xC0000000 + i, 2);
    }

    enable_paging();
}

void map_page_pse(void* physical_address, void* virtual_address, unsigned int flags) {
    unsigned int page_dir_index = (unsigned int) virtual_address / 0x400000;

    unsigned int* page_dir = (unsigned int*)(PAGE_DIR_ADDRESS);

    if(page_dir[page_dir_index] & 1) {
        io_printf("Paging error: pse index already assigned...we should do something about that\n");
    }
    page_dir[page_dir_index] = (unsigned int)physical_address | 0x81 | flags;
    asm("mov eax, 0 \n \
         invlpg byte ptr [eax]");
}

void unmap_page_pse(void* virtual_address) {
    unsigned int page_dir_index = (unsigned int)virtual_address >> 22;
    unsigned int* page_dir = (unsigned int*)PAGE_DIR_ADDRESS;
    page_dir[page_dir_index] = 0x2;

    asm("mov eax, 0 \n \
         invlpg byte ptr [eax]");
}

void map_page(void* physical_address, void* virtual_address, unsigned int flags) {
    unsigned int page_dir_index = (unsigned int)virtual_address / 0x400000;
    unsigned int page_table_index = ((unsigned int)virtual_address >> 12) & 0x3FF;

    unsigned int* page_dir = (unsigned int*)(PAGE_DIR_ADDRESS);
    unsigned int* page_table = (unsigned int*)(PAGE_TABLE_ADDRESS + 0x1000 * page_dir_index);
    unsigned int* page_table_phys = (unsigned int*)(0x400000 + 0x1000 * page_dir_index);

    // Check if the page directory entry is present. If not, create a new table.
    if(!(page_dir[page_dir_index] & 1)) {
        for(int i = 0; i < 0x400; i++) {
            // Mark not present
            page_table[i] = 2;
        }
        page_dir[page_dir_index] = (unsigned int)page_table_phys | 3;
    }

    if(page_table[page_table_index] & 1) {
        io_printf("Paging error: page table already assigned...we should do something about that\n");
        return;
    }

    page_table[page_table_index] = ((unsigned int) physical_address) | (flags & 0xFFF) | 0x01;

    asm("mov eax, 0 \n \
         invlpg byte ptr [eax]");
}

void unmap_page(void* virtual_address) {
    unsigned int page_dir_index = (unsigned int)virtual_address / 0x400000;
    unsigned int page_table_index = ((unsigned int)virtual_address >> 12) & 0x3FF;
    unsigned int* page_table = (unsigned int*)(PAGE_TABLE_ADDRESS + 0x1000 * page_dir_index);
    page_table[page_table_index] = 0;

    asm("mov eax, 0 \n \
         invlpg byte ptr [eax]");
}

void enable_paging() {
    _directory_address = PAGE_PHYSICAL_ADDRESS;
    asm("mov eax, _directory_address     \n \
         mov cr3, eax               \n \
         mov eax, cr0               \n \
         or eax, 0x80000001         \n \
         mov cr0, eax               \n");
}

void disable_paging() {
    asm("mov eax, 0x60000011        \n \
         mov cr0, eax               \n \
         mov eax, 0                 \n \
         mov cr3, eax               \n ");
}

void* get_next_available_virtual_frame(unsigned int count) {
    unsigned int cur_count = 0;
    void *address = NULL;
    for(int i = 3; i < 1024; i++) {
        unsigned int* table_ptr = (unsigned int*)(PAGE_TABLE_ADDRESS + 0x1000 * i);
        for(int j = 0; j < 1024; j++) {
            if(!(table_ptr[j] & 1)) {
                if(cur_count == 0) {
                    address = (void*)(0x400000 * i + j * 0x1000);
                }
                cur_count++;
                if(cur_count == count) {
                    return address;
                }
            } else {
                address = NULL;
                cur_count = 0;
            }
        }
    }

    return (void*)(0);
}

void *allocate_vframe() {
    void *frame = allocate_frame();
    void *vframe = get_next_available_virtual_frame(1);

    map_page(frame, vframe, 2);
    return vframe;
}

void *allocate_contiguous_vframe(size_t size) {
    unsigned int num_of_frames = (size / FRAME_SIZE);
    if(size % FRAME_SIZE) {
        num_of_frames++;
    }
    void *vframe = get_next_available_virtual_frame(num_of_frames);

    for(int i = 0; i < num_of_frames; i++) {
        map_page(allocate_frame(), vframe + FRAME_SIZE * i, 2);
    }

    return vframe;
}

void deallocate_vframe(void *vframe) {
    unmap_page(vframe);
}

void deallocate_contiguous_vframe(void *vframe, size_t size) {
    unsigned int num_of_frames = (size / FRAME_SIZE);
    if(size % FRAME_SIZE) {
        num_of_frames++;
    }

    for(int i = 0; i < num_of_frames; i++) {
        unmap_page(vframe + FRAME_SIZE * i);
    }
}

struct frame_header *create_kernel_frame(size_t size) {
    size_t normalized_size = (size / FRAME_SIZE);
    if(size % FRAME_SIZE) {
        normalized_size++;
    }
    normalized_size *= FRAME_SIZE;

    struct frame_header *next_free = kernel_context.frames;
    while(next_free->next != NULL) {
        next_free = next_free->next;
    }

    struct frame_info *info_frame = allocate_vframe();
    void *data_frame;
    if(size < FRAME_SIZE) {
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
    while(head->used != -1 && head->junk == 0) {
        head++;
    }
    return head;
}

int get_available_allocation(struct frame_header *header, size_t size) {
    struct allocate_info *alloc_info = header->alloc_addr;
    struct allocate_info *available = NULL;

    if(alloc_info == NULL) {
        return NULL;
    }

    while(available == NULL && alloc_info->used != -1) {
        if(alloc_info->size >= size && alloc_info->used == 0) {
            available = alloc_info;
        }
        alloc_info++;
    }
    return available;
}

void *kmalloc(size_t size) {
    if(size == 0) {
        return (void *)(0);
    }

    /* TODO: should check for being kernel. Who really needs security B) */
    struct frame_header *header = kernel_context.frames;
    struct allocate_info *available = NULL;
    while(header != NULL && available == NULL) {
        available = get_available_allocation(header, size);
        header = header->next;
    }

    if(header == NULL) {
        header = create_kernel_frame(size);
        available = get_available_allocation(header, size);
    }

    if(available == NULL || available->used == -1) {
        io_printf("Error: Out of memory...\n");
        return (void *)(0);
    }

    available->used = 1;

    struct allocate_info *next = find_next_junk(header->alloc_addr);
    if(next->used == -1) {
        (next + 1)->used = -1;
    }
    next->used = 0;
    next->junk = 0;
    next->address = available->address + size;
    next->size = available->size - size;
    available->size = size;

    return available->address;
}

void merge_postfix(struct allocate_info *head, struct allocate_info *current) {
    void *next_address = current->address + current->size;
    while(head->used != -1
        && head->address != next_address
        || head->used != 0
        || head->junk != 0){
        head++;
    }

    if(head->used != -1) {
        current->size += head->size;
        if((head + 1)->used == -1) {
            head->used = -1;
        } else {
            head->junk = 1;
        }
    }
}

void merge_prefix(struct allocate_info *head, struct allocate_info *current) {
    void *next_address = current->address;
    while(head->used != -1
        && ((head->address + head->size) != next_address
        || head->used != 0
        || head->junk != 0)) {
        head++;
    }

    if(head->used != -1) {
        current->address = head->address;
        current->size += head->size;
        if((head + 1)->used == -1) {
            head->used = -1;
        } else {
            head->junk = 1;
        }
    }
}

void merge_free_blocks(struct allocate_info *head, struct allocate_info *current) {
    merge_postfix(head, current);
    merge_prefix(head, current);
}

void kfree(void *address) {
    if(address == NULL) {
        return;
    }

    // Look for correct frame of memory.
    struct frame_header *head = kernel_context.frames;
    while(head != NULL && (head->frame_addr != ((unsigned int)address & 0xFFFFF000))) {
        head = head->next;
    }

    // Throw error if frame is unavailable.
    if(head == NULL) {
        io_printf("Error: Freeing unavailable memory...\n");
        return;
    }

    // Look for correct alloaction.
    struct allocate_info *alloc_frame = head->alloc_addr;

    while(alloc_frame->used != -1
        && (alloc_frame->address != address
        || alloc_frame->used != 1
        || alloc_frame->junk != 0)) {
            alloc_frame++;
    }

    // Throw error if allocation is unavailable.
    if(alloc_frame->used == -1) {
        io_printf("Error: Freeing unavailable memory...\n");
        return;
    }

    // Free the memory and merge with surrounding blocks.
    alloc_frame->used = 0;
    merge_free_blocks(head->alloc_addr, alloc_frame);

    // Check for frame deallocation.
    if(head->alloc_addr->used == 0 && (head->alloc_addr + 1)->used == -1) {
        head->prev->next = head->next;
        if(head->next != NULL) {
            head->next->prev = head->prev;
        }
        size_t size = head->alloc_addr->size;
        deallocate_contiguous_vframe(head->frame_addr, size);
        deallocate_vframe(head);
    }
}
