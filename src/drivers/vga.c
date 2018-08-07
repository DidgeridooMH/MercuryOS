#include "vga.h"
#include "cursor.h"

void vga_putchar(unsigned char c) {
  unsigned short entry = (color_mode.bg << 12) | (color_mode.fg << 8) | c;
  unsigned short* dst = vidmem + cursor_position.x + (cursor_position.y * video_mode.width);

  /*
  * If a new line character is to be written,
  * Simply place the cursor out of bounds to force
  * it to move to the next line.
  */
  if(c == '\n') {
    cursor_position.x = 81;
  } else if(c == '\b') {
    *dst--;
    *dst = 0;
    cursor_position.x -= 2;
  } else {
    *dst = entry;
  }

  if(cursor_position.x > 80) {
    cursor_position.x = 0;
    cursor_position.y++;
  } else {
    cursor_position.x++;
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

  cursor_move(x, y);
}

void vga_clear(void) {
  vga_set_cursor(0, 0);
  for(int y = 0; y < video_mode.height; y++) {
    for(int x = 0; x < video_mode.width; x++) {
      vga_putchar(' ');
    }
  }
  vga_set_cursor(0, 0);
}

void vga_init(void) {
  video_mode.width = 80;
  video_mode.height = 25;
  vga_set_color(0x0F, 0x00);
  vga_set_cursor(0, 0);
  vga_clear();
  vga_set_cursor(0, 0);

  //cursor_enable(0, 0xFF);
}

void printf(char* str) {
  int index = 0;
  while(str[index] != '\0') {
    vga_putchar(str[index]);
    index++;
  }
}
