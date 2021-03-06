#include "io.h"
#include "../kernel/memory/mmu.h"
#include "../runtime/itoa.h"
#include "../runtime/stdarg.h"

static unsigned short* video_memory_addr =
    (unsigned short*)VIDEO_MEMORY_ADDRESS;

void io_init() {
    buffer_mode.width = 80;
    buffer_mode.height = 25;
    io_set_color(0x0F, 0x00);
    io_clear_screen();
    io_cursor_enable(0, 15);
}

void io_outportb(unsigned short port, unsigned char data) {
    __asm__ __volatile__("outb %0, %1" : : "dN"(port), "a"(data));
}

unsigned char io_inportb(unsigned short port) {
    unsigned char ret;
    __asm__ __volatile__("inb %0, %1" : "=a"(ret) : "dN"(port));
    return ret;
}

void io_outportw(unsigned short port, unsigned short data) {
    __asm__ __volatile__("outw %0, %1" : : "dN"(port), "a"(data));
}

unsigned short io_inportw(unsigned short port) {
    unsigned short ret;
    __asm__ __volatile__("inw %0, %1" : "=a"(ret) : "dN"(port));
    return ret;
}

void io_outportsw(unsigned short port, unsigned char* data, int length) {
    for (int i = 0; i < length; i++) {
        io_outportw(port, *(((unsigned short*)data) + i));
    }
}

void io_inportsw(unsigned short port, unsigned char* data, int length) {
    for (int i = 0; i < length; i++) {
        ((unsigned short*)data)[i] = io_inportw(port);
    }
}

void io_put_char(unsigned char c) {
    unsigned short entry = (color_mode.bg << 12) | (color_mode.fg << 8) | c;
    unsigned short* dst = video_memory_addr + cursor_position.x +
                          (cursor_position.y * buffer_mode.width);

    /*
     * If a new line character is to be written,
     * Simply place the cursor out of bounds to force
     * it to move to the next line.
     */
    if (c == '\n') {
        cursor_position.x = 80;
    } else if (c == '\b') {
        dst--;
        *dst = entry & ~0xFF;
        cursor_position.x--;
    } else {
        *dst = entry;
        cursor_position.x++;
    }

    if (cursor_position.x > 79) {
        cursor_position.x = 0;
        cursor_position.y++;
        if (cursor_position.y > 24) {
            cursor_position.y--;
            io_scroll_screen();
        }
    }

    io_set_cursor(cursor_position.x, cursor_position.y);
}

void io_set_color(char fg, char bg) {
    color_mode.fg = fg;
    color_mode.bg = bg;
}

void io_set_cursor(int x, int y) {
    cursor_position.x = x;
    cursor_position.y = y;

    io_cursor_move(x, y);
}

void io_scroll_screen() {
    for (int y = 0; y < buffer_mode.height; y++) {
        for (int i = 0; i < buffer_mode.width; i++) {
            video_memory_addr[i + y * buffer_mode.width] =
                video_memory_addr[i + (y + 1) * buffer_mode.width];
        }
    }

    for (int i = 0; i < buffer_mode.width; i++) {
        video_memory_addr[i + buffer_mode.width * 24] = 0;
    }
}

void io_clear_screen() {
    io_set_cursor(0, 0);
    for (int y = 0; y < buffer_mode.height; y++) {
        for (int x = 0; x < buffer_mode.width; x++) {
            io_put_char(' ');
        }
    }
    io_put_char(' ');
    io_set_cursor(0, 0);
}

void io_print_i(int i) {
    char num[16];
    if (i < 0) {
        io_put_char('-');
        i *= -1;
    }
    itoa(num, i, 10);
    io_printf(num);
}

void io_print_o(int i) {
    char num[16];
    itoa(num, i, 8);
    io_printf(num);
}

void io_print_u(unsigned int u) {
    char num[16];
    itoa(num, u, 10);
    io_printf(num);
}

void io_print_x(int x) {
    char num[32];
    itoa(num, x, 16);
    io_printf(num);
}

void io_printf(char* format, ...) {
    va_list args;
    va_start(args, format);
    int index = 0;
    while (format[index] != '\0') {
        if (format[index] == '%') {
            index++;
            switch (format[index]) {
                case 'c':
                    io_put_char(va_arg(args, char));
                    break;
                case 'd':
                case 'i':
                    io_print_i(va_arg(args, int));
                    break;
                case 'o':
                    io_print_o(va_arg(args, int));
                    break;
                case 's':
                    io_printf(va_arg(args, char*));
                    break;
                case 'u':
                    io_print_u(va_arg(args, unsigned int));
                    break;
                case 'x':
                    io_print_x(va_arg(args, int));
                    break;
                case '%':
                    io_put_char('%');
                    break;
                default:
                    // TODO: Maybe throw an error?
                    break;
            }
        } else {
            io_put_char(format[index]);
        }
        index++;
    }
}

void io_cursor_enable(unsigned char start, unsigned char end) {
    io_outportb(0x3D4, 0x0A);
    io_outportb(0x3D5, (io_inportb(0x3D5) & 0xC0) | start);

    io_outportb(0x3D4, 0x0B);
    io_outportb(0x3D5, (io_inportb(0x3D5) & 0xE0) | end);
}

void io_cursor_disable() {
    io_outportb(0x3D4, 0x0A);
    io_outportb(0x3D5, 0x20);
}

void io_cursor_move(int x, int y) {
    unsigned short position = buffer_mode.width * y + x;

    io_outportb(0x3D4, 0x0F);
    io_outportb(0x3D5, (unsigned char)(position & 0xFF));
    io_outportb(0x3D4, 0x0E);
    io_outportb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
}
