#include "vga.h"

void vga_putchar(unsigned char c) {
  unsigned short entry = (color_mode.bg << 12) | (color_mode.fg << 8) | c;
  unsigned short* dst = vidmem + cursor_position.x + (cursor_position.y * video_mode.width);
  *dst = entry;
  if(cursor_position.x > 80) {
    cursor_position.x = 0;
    cursor_position.y++;
  } else {
    cursor_position.x++;
  }
}

void vga_set_color(char fg, char bg) {
  color_mode.fg = fg;
  color_mode.bg = bg;
}

void vga_set_cursor(int x, int y) {
  cursor_position.x = x;
  cursor_position.y = y;
}

void vga_clear(void) {
  for(int y = 0; y < video_mode.height; y++) {
    for(int x = 0; x < video_mode.width; x++) {
      vga_putchar(' ');
    }
  }
}

void vga_init(void) {
  video_mode.width = 80;
  video_mode.height = 25;
  vga_set_color(0x0F, 0x00);
  vga_set_cursor(0, 0);
  vga_clear();
  vga_set_cursor(0, 0);
}

void printf(char* str) {
  while(*str) {
    vga_putchar(*str);
    str++;
  }
}
