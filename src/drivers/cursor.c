#include "cursor.h"
#include "vga.h"

void cursor_enable(unsigned char start, unsigned char end) {
  outportb(0x3D4, 0x0A);
  outportb(0x3D5, (inportb(0x3D5) & 0xC0) | start);

	outportb(0x3D4, 0x0B);
	outportb(0x3D5, (inportb(0x3E0) & 0xE0) | end);
}

void cursor_disable() {
	outportb(0x3D4, 0x0A);
	outportb(0x3D5, 0x20);
}

void cursor_move(int x, int y) {
  unsigned short position = video_mode.width * y + x;

  outportb(0x3D4, 0x0F);
  outportb(0x3D5, (unsigned char)(position & 0xFF));
  outportb(0x3D4, 0x0E);
  outportb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
}
