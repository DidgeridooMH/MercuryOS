void itoa(char *buf, unsigned long int n, int base) {
    int ptr = 0;

    do {
        unsigned int currentNum = n % base;
        buf[ptr++] =
            (currentNum < 10) ? (currentNum + '0') : (currentNum - 10 + 'a');
    } while (n /= base);

    buf[ptr] = 0;

    int ptr2 = 0;
    ptr--;

    while (ptr2 < ptr) {
        int tmp = buf[ptr2];
        buf[ptr2] = buf[ptr];
        buf[ptr] = tmp;

        ptr--;
        ptr2++;
    }
}
