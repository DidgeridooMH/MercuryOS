#ifndef OS_H
#define OS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* str);

static inline void outb(uint16_t port, uint8_t val) {
  asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  asm volatile("inb %1, %0"
                : "=a"(ret)
                : "Nd"(port));
  return ret;
}

static inline void lidt(void* base, uint16_t size) {
    struct {
        uint16_t length;
        void*    base;
    } __attribute__((packed)) IDTR = { size, base };

    asm ( "lidt %0" : : "m"(IDTR) );
}

#endif
