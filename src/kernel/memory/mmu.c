#include "mmu.h"
#include "../../drivers/io.h"
#include "../../runtime/itoa.h"
#include "../../runtime/memory.h"
#include "../arch/x86/x86.h"
#include "frame.h"

static struct memory_context kernel_context;
static struct frame_header kernel_frame_head;

void memory_init() {
    paging_load();

    setup_frame_alloc();

    kernel_context.id = 0;
    kernel_context.frames = &kernel_frame_head;

    kernel_frame_head.frame_addr = NULL;
    kernel_frame_head.alloc_addr = NULL;
    kernel_frame_head.next = NULL;
    kernel_frame_head.prev = NULL;
}

struct memory_context *get_kernel_context() {
    return &kernel_context;
}

static void print_list(struct frame_header *head) {
    while (head != NULL) {
        io_printf("0x%x -> ", (unsigned int)head);
        head = head->next;
    }
    io_printf("NULL\n");
}

void *kmalloc(unsigned int size) {
    if (size == 0) {
        return (void *)(0);
    }

    /* TODO: should check for being kernel. Who really needs security B) */
    struct frame_header *header = kernel_context.frames;
    struct allocate_info *available = NULL;
    while (header != NULL && available == NULL) {
        available = get_available_allocation(header, size);
        header = header->next;
    }

    if (header == NULL) {
        header = create_kernel_frame(size);
        available = get_available_allocation(header, size);
    }

    if (available == NULL || available->used == -1) {
        io_printf("Error: Out of memory...\n");
        return (void *)(0);
    }

    available->used = 1;

    struct allocate_info *next = find_next_junk(header->alloc_addr);
    if (next->used == -1) {
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

    while (head->used != -1 && (head->address != next_address ||
                                head->used != 0 || head->junk != 0)) {
        head++;
    }

    if (head->used != -1) {
        current->size += head->size;
        if ((head + 1)->used == -1) {
            head->used = -1;
        } else {
            head->junk = 1;
        }
    }
}

void merge_prefix(struct allocate_info *head, struct allocate_info *current) {
    void *next_address = current->address;
    while (head->used != -1 && ((head->address + head->size) != next_address ||
                                head->used != 0 || head->junk != 0)) {
        head++;
    }

    if (head->used != -1) {
        current->address = head->address;
        current->size += head->size;
        if ((head + 1)->used == -1) {
            head->used = -1;
        } else {
            head->junk = 1;
        }
    }
}

void merge_free_blocks(struct allocate_info *head,
                       struct allocate_info *current) {
    merge_postfix(head, current);
    merge_prefix(head, current);
}

void kfree(void *address) {
    if (address == NULL) {
        return;
    }

    // Look for correct frame of memory.
    struct frame_header *head = kernel_context.frames;
    while (head != NULL &&
           (head->frame_addr != ((unsigned int)address & 0xFFFFF000))) {
        head = head->next;
    }

    // Throw error if frame is unavailable.
    if (head == NULL) {
        io_printf("Error: Freeing unavailable memory...\n");
        return;
    }

    // Look for correct alloaction.
    struct allocate_info *alloc_frame = head->alloc_addr;

    while (alloc_frame->used != -1 &&
           (alloc_frame->address != address || alloc_frame->used != 1 ||
            alloc_frame->junk != 0)) {
        alloc_frame++;
    }

    // Throw error if allocation is unavailable.
    if (alloc_frame->used == -1) {
        io_printf("Error: Freeing unavailable memory...\n");
        return;
    }

    // Free the memory and merge with surrounding blocks.
    alloc_frame->used = 0;
    merge_free_blocks(head->alloc_addr, alloc_frame);

    // Check for frame deallocation.
    if (head->alloc_addr->used == 0 && (head->alloc_addr + 1)->used == -1) {
        head->prev->next = head->next;
        if (head->next != NULL) {
            head->next->prev = head->prev;
        }
        unsigned int size = head->alloc_addr->size;
        deallocate_contiguous_vframe(head->frame_addr, size);
        deallocate_vframe(head);
    }
}
