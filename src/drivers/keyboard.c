#include "keyboard.h"

void keyboard_handler(struct regs* r) {
  unsigned char scancode = inportb(0x60);

  if(scancode & 0x80) {
    // Released
  } else {
    vga_putchar(kbdus[scancode]);
  }
}

void keyboard_install() {
  irq_install_handler(1, keyboard_handler);
}
