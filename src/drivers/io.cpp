#include "io.h"

struct BufferMode video_mode;
struct ColorMode  color_mode;
struct Cursor     cursor_position;

unsigned short* video_memory = (unsigned short*)0xB8000;

void Io::init() {
  video_mode.width = 80;
  video_mode.height = 25;
  set_color(0x0F, 0x00);
  clear_screen();

  cursor_enable(0, 15);
}

void Io::outportb(unsigned short port, unsigned char data) {
  __asm__ __volatile__ ("outb %0, %1" : : "dN" (port), "a" (data));
}

unsigned char Io::inportb(unsigned short port) {
  unsigned char ret;
  __asm__ __volatile__ ("inb %0, %1" : "=a" (ret) : "dN" (port));
  return ret;
}

void Io::put_char(unsigned char c) {
  unsigned short entry = (color_mode.bg << 12) | (color_mode.fg << 8) | c;
  unsigned short* dst = video_memory + cursor_position.x + (cursor_position.y * video_mode.width);

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
      scroll_screen();
    }
  }

  set_cursor(cursor_position.x, cursor_position.y);
}

void Io::set_color(char fg, char bg) {
  color_mode.fg = fg;
  color_mode.bg = bg;
}

void Io::set_cursor(int x, int y) {
  cursor_position.x = x;
  cursor_position.y = y;

  cursor_move(x, y);
}

void Io::scroll_screen() {
  for(int y = 0; y < video_mode.height; y++){
    for(int i = 0; i < video_mode.width; i++) {
      video_memory[i + y * video_mode.width] = video_memory[i + (y + 1) * video_mode.width];
    }
  }

  for(int i = 0; i < video_mode.width; i++) {
    video_memory[i + video_mode.width * 24] = 0;
  }
}

void Io::clear_screen() {
  set_cursor(0, 0);
  for(int y = 0; y < video_mode.height; y++) {
    for(int x = 0; x < video_mode.width; x++) {
      put_char(' ');
    }
  }
  put_char(' ');
  set_cursor(0, 0);
}

void Io::printf(const char* str) {
  int index = 0;
  while(str[index] != '\0') {
    put_char(str[index]);
    index++;
  }
}

void Io::cursor_enable(unsigned char start, unsigned char end) {
    outportb(0x3D4, 0x0A);
    outportb(0x3D5, (inportb(0x3D5) & 0xC0) | start);

	outportb(0x3D4, 0x0B);
	outportb(0x3D5, (inportb(0x3E0) & 0xE0) | end);
}

void Io::cursor_disable() {
	outportb(0x3D4, 0x0A);
	outportb(0x3D5, 0x20);
}

void Io::cursor_move(int x, int y) {
  unsigned short position = video_mode.width * y + x;

  outportb(0x3D4, 0x0F);
  outportb(0x3D5, (unsigned char)(position & 0xFF));
  outportb(0x3D4, 0x0E);
  outportb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
}
