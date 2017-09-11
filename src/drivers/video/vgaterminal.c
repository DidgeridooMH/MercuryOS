#include "vgaterminal.h"

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll(void) {
	for(int y = 0; y < VGA_HEIGHT - 1; y++) {
		for(int x = 0; x < VGA_WIDTH - 1; x++) {
			terminal_buffer[x + (y * VGA_WIDTH)] = terminal_buffer[x + ((y + 1) * VGA_WIDTH)];
		}
	}
	for(int x = 0; x < VGA_WIDTH - 1; x++) {
		terminal_buffer[x + ((VGA_HEIGHT - 1) * VGA_WIDTH)] = vga_entry(' ', terminal_color);
	}
}

void terminal_putchar(char c) {
	if(c == '\n') {
		terminal_row++;
		terminal_column = 0;
		if(terminal_row == VGA_HEIGHT) {
			terminal_scroll();
			terminal_row--;
		}
		return;
	} else {
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			if (++terminal_row == VGA_HEIGHT) {
				terminal_scroll();
				terminal_row--;
			}
		}
	}
}

void terminal_write(const char* data, size_t size) {

	for (size_t i = 0; i < size; i++) {
		terminal_putchar(data[i]);
	}
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
	if(terminal_column == VGA_WIDTH) {
		terminal_updatecursor(terminal_row + 0x01, 0x0);
	} else if(terminal_row == VGA_HEIGHT) {
		terminal_updatecursor(0x0, 0x0);
	} else {
		terminal_updatecursor(terminal_row, terminal_column);
	}
}

void terminal_writestring_s(const char* data,
													enum vga_color fg,
													enum vga_color bg) {
	uint8_t color = terminal_color;
	terminal_setcolor(vga_entry_color(fg, bg));

	terminal_write(data, strlen(data));
	if(terminal_column == VGA_WIDTH) {
		terminal_updatecursor(terminal_row + 0x01, 0x0);
	} else if(terminal_row == VGA_HEIGHT) {
		terminal_updatecursor(0x0, 0x0);
	} else {
		terminal_updatecursor(terminal_row, terminal_column);
	}

	terminal_color = color;
}

void terminal_updatecursor(uint16_t row, uint16_t column) {
	uint16_t position = (row * 80) + column;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (unsigned char)(position & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
}
