#include "io.h"

static unsigned short* video_memory_addr = (unsigned short*)0xB8000;

void io_init() {
  buffer_mode.width = 80;
  buffer_mode.height = 25;
  io_set_color(0x0F, 0x00);
  io_clear_screen();
  io_cursor_enable(0, 15);
}

void io_outportb(unsigned int port, unsigned char data) {
  __asm__ __volatile__ ("outb %%al, %%dx" : : "d" (port), "a" (data));
}

unsigned char io_inportb(unsigned int port) {
  unsigned char ret;
  __asm__ __volatile__ ("inb %%dx, %%al" : "=a" (ret) : "d" (port));
  return ret;
}

void io_put_char(unsigned char c) {
  unsigned short entry = (color_mode.bg << 12) | (color_mode.fg << 8) | c;
  unsigned short* dst = video_memory_addr + cursor_position.x + (cursor_position.y * buffer_mode.width);

  /*
  * If a new line character is to be written,
  * Simply place the cursor out of bounds to force
  * it to move to the next line.
  */
  if(c == '\n') {
    cursor_position.x = 80;
  } else if(c == '\b') {
    dst--;
    *dst = entry & ~0xFF;
    cursor_position.x--;
  } else {
    *dst = entry;
    cursor_position.x++;
  }

  if(cursor_position.x > 79) {
    cursor_position.x = 0;
    cursor_position.y++;
    if(cursor_position.y > 24) {
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
  for(int y = 0; y < buffer_mode.height; y++){
    for(int i = 0; i < buffer_mode.width; i++) {
      video_memory_addr[i + y * buffer_mode.width] = video_memory_addr[i + (y + 1) * buffer_mode.width];
    }
  }

  for(int i = 0; i < buffer_mode.width; i++) {
    video_memory_addr[i + buffer_mode.width * 24] = 0;
  }
}

void io_clear_screen() {
  io_set_cursor(0, 0);
  for(int y = 0; y < buffer_mode.height; y++) {
    for(int x = 0; x < buffer_mode.width; x++) {
      io_put_char(' ');
    }
  }
  io_put_char(' ');
  io_set_cursor(0, 0);
}

void io_printf(const char* str) {
  int index = 0;
  while(str[index] != '\0') {
    io_put_char(str[index]);
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
