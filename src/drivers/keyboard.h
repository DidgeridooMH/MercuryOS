#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../kernel/arch/x86/x86.h"

struct keyboard_flags {
    unsigned char shift;
    unsigned char ctrl;
    unsigned char alt;
    unsigned char caps_lock;
    unsigned char num_lock;
    unsigned char scroll_lock;
};

void keyboard_handler(struct regs* r);

unsigned int keyboard_get_pointer();

void keyboard_update_flags(unsigned char code);

void keyboard_push(unsigned char data);

unsigned char keyboard_pull();

void keyboard_install();

unsigned char keyboard_get_char(int keycode);

#endif
