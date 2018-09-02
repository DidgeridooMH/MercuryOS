#include "io.h"

Io::Io() {
  m_videoMode.width = 80;
  m_videoMode.height = 25;
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
  unsigned short entry = (m_colorMode.bg << 12) | (m_colorMode.fg << 8) | c;
  unsigned short* dst = m_vidmem + m_cursorPosition.x + (m_cursorPosition.y * m_videoMode.width);

  /*
  * If a new line character is to be written,
  * Simply place the cursor out of bounds to force
  * it to move to the next line.
  */
  if(c == '\n') {
    m_cursorPosition.x = 80;
  } else if(c == '\b') {
    dst--;
    *dst = entry & ~0xFF;
    m_cursorPosition.x--;
  } else {
    *dst = entry;
    m_cursorPosition.x++;
  }

  if(m_cursorPosition.x > 79) {
    m_cursorPosition.x = 0;
    m_cursorPosition.y++;
    if(m_cursorPosition.y > 24) {
      m_cursorPosition.y--;
      scrollScreen();
    }
  }

  setCursor(m_cursorPosition.x, m_cursorPosition.y);
}

void Io::setColor(char fg, char bg) {
  m_colorMode.fg = fg;
  m_colorMode.bg = bg;
}

void Io::setCursor(int x, int y) {
  m_cursorPosition.x = x;
  m_cursorPosition.y = y;

  cursorMove(x, y);
}

void Io::scrollScreen() {
  for(int y = 0; y < m_videoMode.height; y++){
    for(int i = 0; i < m_videoMode.width; i++) {
      m_vidmem[i + y * m_videoMode.width] = m_vidmem[i + (y + 1) * m_videoMode.width];
    }
  }

  for(int i = 0; i < m_videoMode.width; i++) {
    m_vidmem[i + m_videoMode.width * 24] = 0;
  }
}

void Io::clearScreen() {
  setCursor(0, 0);
  for(int y = 0; y < m_videoMode.height; y++) {
    for(int x = 0; x < m_videoMode.width; x++) {
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
    io.outportb(0x3D4, 0x0A);
    io.outportb(0x3D5, (io.inportb(0x3D5) & 0xC0) | start);

	io.outportb(0x3D4, 0x0B);
	io.outportb(0x3D5, (io.inportb(0x3E0) & 0xE0) | end);
}

void Io::cursorDisable() {
	io.outportb(0x3D4, 0x0A);
	io.outportb(0x3D5, 0x20);
}

void Io::cursorMove(int x, int y) {
  unsigned short position = m_videoMode.width * y + x;

  io.outportb(0x3D4, 0x0F);
  io.outportb(0x3D5, (unsigned char)(position & 0xFF));
  io.outportb(0x3D4, 0x0E);
  io.outportb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
}
