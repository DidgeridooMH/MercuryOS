#include "keyboard.h"

void keyboard_handler(struct regs* r) {
  unsigned char scancode = inportb(0x60);

  if(scancode & 0x80) {
    update_flags(scancode & 0x7F);
  } else {
    if(kbdus[scancode] > 0) {
      if(kb.shift) {
        keyboard_push(kbdus_shifted[scancode]);
      } else {
        keyboard_push(kbdus[scancode]);
      }
    }
    update_flags(scancode);
  }
}

void update_flags(unsigned char code) {
  switch(code) {
    case 0x2A:
    case 0x36:
      kb.shift = !kb.shift;
      break;
    case 0x1D:
      kb.ctrl = !kb.ctrl;
      break;
    case 0xB8:
    case 0x38:
      kb.alt = !kb.alt;
      break;
  }
}

unsigned int get_keyboard_pointer() {
  return keyboard_pointer;
}

void keyboard_push(unsigned char data) {
  keyboard_buffer[keyboard_pointer] = data;
  keyboard_pointer++;
  keyboard_pointer %= 512;
}

unsigned char keyboard_pull() {
  if(keyboard_pointer == 0) {
    keyboard_pointer = 511;
  } else {
    keyboard_pointer--;
  }

  return keyboard_buffer[keyboard_pointer];
}

void keyboard_install() {
  irq_install_handler(1, keyboard_handler);
  memset(keyboard_buffer, 0, 512);
}
