// Copyright (C) - Tillitis AB
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <tkey/lib.h>
#include <tkey/qemu_debug.h>
#include <tkey/tk1_mem.h>

// clang-format off
static volatile uint8_t* const debugtx = (volatile uint8_t *)TK1_MMIO_QEMU_DEBUG;
// clang-format on

int qemu_putchar(const uint8_t ch)
{
	*debugtx = ch;

	return ch;
}

void qemu_lf()
{
	qemu_putchar('\n');
}

static char hexnibble(const uint8_t ch)
{
	switch (ch) {
	case 0x0:
		return '0';
	case 0x1:
		return '1';
	case 0x2:
		return '2';
	case 0x3:
		return '3';
	case 0x4:
		return '4';
	case 0x5:
		return '5';
	case 0x6:
		return '6';
	case 0x7:
		return '7';
	case 0x8:
		return '8';
	case 0x9:
		return '9';
	case 0xa:
		return 'a';
	case 0xb:
		return 'b';
	case 0xc:
		return 'c';
	case 0xd:
		return 'd';
	case 0xe:
		return 'e';
	case 0xf:
		return 'f';
	}

	return '0';
}

void qemu_puthex(const uint8_t ch)
{
	qemu_putchar(hexnibble(ch >> 4 & 0x0f));
	qemu_putchar(hexnibble(ch & 0x0f));
}

void qemu_putinthex(const uint32_t n)
{
	uint8_t buf[4];

	memcpy(buf, &n, 4);
	qemu_puts("0x");
	for (int i = 3; i > -1; i--) {
		qemu_puthex(buf[i]);
	}
}

void qemu_puts(const char *s)
{
	while (*s)
		qemu_putchar(*s++);
}

void qemu_hexdump(const uint8_t *buf, int len)
{
	uint8_t *byte_buf = (uint8_t *)buf;

	for (int i = 0; i < len; i++) {
		qemu_puthex(byte_buf[i]);
		if (i % 2 == 1) {
			(void)qemu_putchar(' ');
		}

		if ((i + 1) % 16 == 0) {
			qemu_lf();
		}
	}

	qemu_lf();
}
