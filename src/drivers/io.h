#ifndef IO_H
#define IO_H

struct BufferMode {
  int width;
  int height;
};

struct ColorMode {
  char fg;
  char bg;
};

struct Cursor {
  int x;
  int y;
};

namespace Io {
    void init();
    void set_color(char fg, char bg);
    void clear_screen();
    void put_char(unsigned char c);
    void set_cursor(int x, int y);
    void scroll_screen();
    void outportb(unsigned short port, unsigned char data);
    unsigned char inportb(unsigned short port);
    void printf(const char* str);
    void cursor_enable(unsigned char start, unsigned char end);
    void cursor_disable();
    void cursor_move(int x, int y);
};

#endif
