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

class Io {
public:
    Io();
    void setColor(char fg, char bg);
    void clearScreen();
    void putChar(unsigned char c);
    void setCursor(int x, int y);
    void scrollScreen();
    void outportb(unsigned short port, unsigned char data);
    unsigned char inportb(unsigned short port);

    void printf(const char* str);

private:
    unsigned short* m_vidmem = (unsigned short*)0xB8000;

    struct bufmode m_videoMode;
    struct colormode m_colorMode;
    struct cursor m_cursorPosition;

    void cursorEnable(unsigned char start, unsigned char end);
    void cursorDisable();
    void cursorMove(int x, int y);
};

extern Io io;

#endif
