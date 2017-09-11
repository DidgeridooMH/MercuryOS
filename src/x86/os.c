#include "os.h"

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void memset(void *src, int c, size_t size) {
	unsigned char *srcp = src;
	for(int i = 0; i < size; i++) {
		srcp[i] = c;
	}
}
