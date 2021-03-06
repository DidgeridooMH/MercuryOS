#include "string.h"

int strlen(char *s) {
    int n = 0;
    while (*s++) {
        n++;
    }
    return n;
}

int strcmp(char *dst, char *src) {
    int n = strlen(dst);
    int m = strlen(src);

    if (n > m) {
        return 1;
    } else if (n < m) {
        return -1;
    }

    for (int i = 0; i < n; i++) {
        if (src[i] != dst[i]) {
            if (src[i] > dst[i]) {
                return -1;
            } else {
                return 1;
            }
        }
    }

    return 0;
}

int strcpy(char *dst, char *src) {
    int n = strlen(src);
    for (int i = 0; i < n; i++) {
        dst[i] = src[i];
    }
    return 0;
}

void strcat(char *dest, char *src) {
    int n = strlen(dest);
    int m = strlen(src);
    for (int i = 0; i < m; i++) {
        dest[n + i] = src[i];
    }
    dest[n + m] = '\0';
    return;
}

int strncmp(const char *s1, const char *s2, int n) {
    int result = 0;

    while (n && result == 0) {
        result = *s1 - *s2;

        s1++;
        s2++;
        n--;
    }

    return result;
}
