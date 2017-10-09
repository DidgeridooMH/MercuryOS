#ifndef VGA_H
#define VGA_H

static unsigned short* vidmem = 0xB8000;

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

struct bufmode video_mode;
struct colormode color_mode;
struct cursor cursor_position;

void vga_set_color(char fg, char bg);
void vga_init(void);
void vga_putchar(unsigned char c);
void vga_clear(void);
void vga_set_cursor(int x, int y);

void printf(char* str);

#endif
