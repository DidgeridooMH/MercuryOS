#include "io.h"

struct bufmode videoMode;
struct colormode colorMode;
struct cursor cursorPosition;

unsigned short* vidmem = (unsigned short*)0xB8000;

void Io::init() {
  videoMode.width = 80;
  videoMode.height = 25;
  setColor(0x0F, 0x00);
  clearScreen();

  cursorEnable(0, 15);
}

void Io::outportb(unsigned short port, unsigned char data) {
  __asm__ __volatile__ ("outb %0, %1" : : "dN" (port), "a" (data));
}

unsigned char Io::inportb(unsigned short port) {
  unsigned char ret;
  __asm__ __volatile__ ("inb %0, %1" : "=a" (ret) : "dN" (port));
  return ret;
}

void Io::putChar(unsigned char c) {
  unsigned short entry = (colorMode.bg << 12) | (colorMode.fg << 8) | c;
  unsigned short* dst = vidmem + cursorPosition.x + (cursorPosition.y * videoMode.width);

  /*
  * If a new line character is to be written,
  * Simply place the cursor out of bounds to force
  * it to move to the next line.
  */
  if(c == '\n') {
    cursorPosition.x = 80;
  } else if(c == '\b') {
    dst--;
    *dst = entry & ~0xFF;
    cursorPosition.x--;
  } else {
    *dst = entry;
    cursorPosition.x++;
  }

  if(cursorPosition.x > 79) {
    cursorPosition.x = 0;
    cursorPosition.y++;
    if(cursorPosition.y > 24) {
      cursorPosition.y--;
      scrollScreen();
    }
  }

  setCursor(cursorPosition.x, cursorPosition.y);
}

void Io::setColor(char fg, char bg) {
  colorMode.fg = fg;
  colorMode.bg = bg;
}

void Io::setCursor(int x, int y) {
  cursorPosition.x = x;
  cursorPosition.y = y;

  cursorMove(x, y);
}

void Io::scrollScreen() {
  for(int y = 0; y < videoMode.height; y++){
    for(int i = 0; i < videoMode.width; i++) {
      vidmem[i + y * videoMode.width] = vidmem[i + (y + 1) * videoMode.width];
    }
  }

  for(int i = 0; i < videoMode.width; i++) {
    vidmem[i + videoMode.width * 24] = 0;
  }
}

void Io::clearScreen() {
  setCursor(0, 0);
  for(int y = 0; y < videoMode.height; y++) {
    for(int x = 0; x < videoMode.width; x++) {
      putChar(' ');
    }
  }
  putChar(' ');
  setCursor(0, 0);
}

void Io::printf(const char* str) {
  int index = 0;
  while(str[index] != '\0') {
    putChar(str[index]);
    index++;
  }
}

void Io::cursorEnable(unsigned char start, unsigned char end) {
    outportb(0x3D4, 0x0A);
    outportb(0x3D5, (inportb(0x3D5) & 0xC0) | start);

	outportb(0x3D4, 0x0B);
	outportb(0x3D5, (inportb(0x3E0) & 0xE0) | end);
}

void Io::cursorDisable() {
	outportb(0x3D4, 0x0A);
	outportb(0x3D5, 0x20);
}

void Io::cursorMove(int x, int y) {
  unsigned short position = videoMode.width * y + x;

  outportb(0x3D4, 0x0F);
  outportb(0x3D5, (unsigned char)(position & 0xFF));
  outportb(0x3D4, 0x0E);
  outportb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
}
