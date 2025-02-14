// SPDX-FileCopyrightText: 2025 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <stddef.h>
#include <stdint.h>

#ifndef TKEY_IO_H
#define TKEY_IO_H

enum ioend {
	IO_NONE = 0x00,
	IO_QEMU = 0x10,
	IO_TKEYCTRL = 0x20,
	IO_CDC = 0x40,
	IO_HID = 0x80,
};

void write(enum ioend dest, const uint8_t *buf, size_t nbytes);
int read(enum ioend src, uint8_t *buf, size_t bufsize, size_t nbytes);
enum ioend readselect(int bitmask, uint8_t *bytes_available);
void putchar(enum ioend dest, const uint8_t ch);
void puthex(enum ioend dest, const uint8_t ch);
void putinthex(enum ioend dest, const uint32_t n);
void puts(enum ioend dest, const char *s);
void hexdump(enum ioend dest, void *buf, int len);
#endif
