#ifndef VGA_H
#define VGA_H

struct bufmode {
  int width;
  int height;
};

struct colormode {
  char fg;
  char bg;
};

struct cursor {
  int x;
  int y;
};

void vga_set_color(char fg, char bg);
void vga_init(void);
void vga_putchar(unsigned char c);
void vga_clear(void);
void vga_set_cursor(int x, int y);
void vga_scroll();

void printf(const char* str);

#endif
