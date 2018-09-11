#ifndef IO_H
#define IO_H

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

namespace Io {
    void init();
    void setColor(char fg, char bg);
    void clearScreen();
    void putChar(unsigned char c);
    void setCursor(int x, int y);
    void scrollScreen();
    void outportb(unsigned short port, unsigned char data);
    unsigned char inportb(unsigned short port);
    void printf(const char* str);
    void cursorEnable(unsigned char start, unsigned char end);
    void cursorDisable();
    void cursorMove(int x, int y);
};

#endif
