#include "memory.h"

void* memset(char *dst, char src, int n) {
  char *p = dst;
  while(n--) {
    *dst++ = src;
  }
  return p;
}

void* memcpy(char *dst, char *src, int n) {
  char *p = dst;
  while(n--) {
    *dst++ = *src++;
  }
  return p;
}
