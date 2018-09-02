#ifndef KEYBOARD_H
#define KEYBOARD_H

typedef struct {
  bool shift;
  bool ctrl;
  bool alt;
  bool caps_lock;
  bool num_lock;
  bool scroll_lock;
} kb_flags;

void keyboard_handler(struct regs* r);
void update_flags(unsigned char code);
unsigned int get_keyboard_pointer();
void keyboard_push(unsigned char data);
unsigned char keyboard_pull();
void keyboard_install();
unsigned char get_char(int keycode);

#endif
