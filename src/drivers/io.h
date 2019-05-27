#ifndef IO_H
#define IO_H

#define VIDEO_MEMORY_ADDRESS 0xC00B8000

enum TTYColor {
    TTY_COLOR_BLACK = 0,
    TTY_COLOR_BLUE = 1,
    TTY_COLOR_GREEN = 2,
    TTY_COLOR_CYAN = 3,
    TTY_COLOR_RED = 4,
    TTY_COLOR_MAGENTA = 5,
    TTY_COLOR_BROWN = 6,
    TTY_COLOR_LIGHT_GREY = 7,
    TTY_COLOR_DARK_GREY = 8,
    TTY_COLOR_LIGHT_BLUE = 9,
    TTY_COLOR_LIGHT_GREEN = 10,
    TTY_COLOR_LIGHT_CYAN = 11,
    TTY_COLOR_LIGHT_RED = 12,
    TTY_COLOR_LIGHT_MAGENTA = 13,
    TTY_COLOR_LIGHT_BROWN = 14,
    TTY_COLOR_WHITE = 15,
};

struct io_buffer_mode {
    int width;
    int height;
} buffer_mode;

struct io_color_mode {
    char fg;
    char bg;
} color_mode;

struct io_cursor {
    int x;
    int y;
} cursor_position;

void io_init();

void io_set_color(char fg, char bg);

void io_clear_screen();

void io_put_char(unsigned char c);

void io_set_cursor(int x, int y);

void io_scroll_screen();

void io_outportb(unsigned short port, unsigned char data);

unsigned char io_inportb(unsigned short port);

void io_outportw(unsigned short port, unsigned short data);

unsigned short io_inportw(unsigned short port);

void io_outportsw(unsigned short port, unsigned char* data, int length);

void io_inportsw(unsigned short port, unsigned char* data, int length);

void io_printf(char* format, ...);

void io_cursor_enable(unsigned char start, unsigned char end);

void io_cursor_disable();

void io_cursor_move(int x, int y);

#endif
