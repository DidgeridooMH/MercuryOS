#ifndef CURSOR_H
#define CURSOR_H

void cursor_enable(unsigned char start, unsigned char end);

void cursor_disable();

void cursor_move(int x, int y, int width);

#endif
