#ifndef IO_H
#define IO_H

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

static unsigned short* video_memory_addr = (unsigned short*)0xB8000;

void io_init();

void io_set_color(char fg, char bg);

void io_clear_screen();

void io_put_char(unsigned char c);

void io_set_cursor(int x, int y);

void io_scroll_screen();

void io_outportb(unsigned int port, unsigned char data);

unsigned char io_inportb(unsigned int port);

void io_printf(const char* str);

void io_cursor_enable(unsigned char start, unsigned char end);

void io_cursor_disable();

void io_cursor_move(int x, int y);


#endif
