#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../kernel/system.h"
#include "../runtime/memory.h"

typedef struct {
  bool shift;
  bool ctrl;
  bool alt;
  bool caps_lock;
  bool num_lock;
  bool scroll_lock;
} kb_flags;

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

kb_flags kb = { false, false, false, false, false, false };

unsigned char keyboard_buffer[512];

unsigned int keyboard_pointer = 0;

#endif
