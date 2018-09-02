#include "vga.h"
#include "cursor.h"

static unsigned short* vidmem = (unsigned short*)0xB8000;

struct bufmode video_mode;
struct colormode color_mode;
struct cursor cursor_position;

void vga_putchar(unsigned char c) {
  unsigned short entry = (color_mode.bg << 12) | (color_mode.fg << 8) | c;
  unsigned short* dst = vidmem + cursor_position.x + (cursor_position.y * video_mode.width);

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
      cursor_position.y -= 2;
      vga_scroll();
    }
  }

  vga_set_cursor(cursor_position.x, cursor_position.y);
}

void vga_set_color(char fg, char bg) {
  color_mode.fg = fg;
  color_mode.bg = bg;
}

void vga_set_cursor(int x, int y) {
  cursor_position.x = x;
  cursor_position.y = y;

  cursor_move(x, y, video_mode.width);
}

void vga_scroll() {
  for(int y = 0; y < video_mode.height; y++){
    for(int i = 0; i < video_mode.width; i++) {
      vidmem[i + y * video_mode.width] = vidmem[i + (y + 1) * video_mode.width];
    }
  }

  for(int i = 0; i < video_mode.width; i++) {
    vidmem[i + video_mode.width * 24] = 0;
  }
}

void vga_clear() {
  vga_set_cursor(0, 0);
  for(int y = 0; y < video_mode.height; y++) {
    for(int x = 0; x < video_mode.width; x++) {
      vga_putchar(' ');
    }
  }
  vga_putchar(' ');
  vga_set_cursor(0, 0);
}

void vga_init() {
  video_mode.width = 80;
  video_mode.height = 25;
  vga_set_color(0x0F, 0x00);
  vga_clear();

  cursor_enable(0, 15);
}

void printf(const char* str) {
  int index = 0;
  while(str[index] != '\0') {
    vga_putchar(str[index]);
    index++;
  }
}
