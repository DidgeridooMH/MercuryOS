#ifndef MMU_H
#define MMU_H

#define NULL 0x0

struct memory_context {
    unsigned int id;
    struct frame_header *frames;
};

struct memory_context *get_kernel_context();

void *kmalloc(unsigned int size);

void kfree(void *address);

#endif
