void outportb(unsigned short port, unsigned char data) {
  __asm__ __volatile__ ("outb %0, %1" : : "dN" (port), "a" (data));
}

void inportb(unsigned short port) {
  unsigned char ret;
  __asm__ __volatile__ ("inb %0, %1" : "=a" (ret) : "dN" (port));
}
