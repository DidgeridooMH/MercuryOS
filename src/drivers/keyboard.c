#include "keyboard.h"
#include "io.h"
#include "../runtime/memory.h"

unsigned char keyboard_buffer[512];

unsigned int keyboard_pointer = 0;

struct keyboard_flags kb_flags = { 0, 0, 0, 0, 0, 0 };

unsigned char kbdus[128] = {
    0,  0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	'\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

unsigned char kbdus_shifted[128] = {
    0,  0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',	'\"', '~', 0, '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void keyboard_handler(struct regs* r) {
  unsigned char scancode = io_inportb(0x60);

  if(scancode & 0x80) {
    keyboard_update_flags(scancode & 0x7F);
  } else {
    if(kbdus[scancode] > 0) {
      if(kb_flags.shift) {
        keyboard_push(kbdus_shifted[scancode]);
      } else {
        keyboard_push(kbdus[scancode]);
      }
    }
    keyboard_update_flags(scancode);
  }
}

void keyboard_update_flags(unsigned char code) {
  switch(code) {
    case 0x2A:
    case 0x36:
      kb_flags.shift = !kb_flags.shift;
      break;
    case 0x1D:
      kb_flags.ctrl = !kb_flags.ctrl;
      break;
    case 0xB8:
    case 0x38:
      kb_flags.alt = !kb_flags.alt;
      break;
  }
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
  memset((char*)keyboard_buffer, 0, 512);
}

unsigned char keyboard_get_char(int keycode) {
  return kbdus[keycode];
}
