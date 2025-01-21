// SPDX-FileCopyrightText: 2023 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <tkey/lib.h>
#include <tkey/proto.h>
#include <tkey/tk1_mem.h>

#ifndef TKEY_DEBUG
#define TKEY_DEBUG
#endif

#include <tkey/debug.h>

#define PACKET_SIZE 64
#define HEADER_SIZE 2

// clang-format off
static volatile uint32_t *can_tx = (volatile uint32_t *)TK1_MMIO_UART_TX_STATUS;
static volatile uint32_t *tx =     (volatile uint32_t *)TK1_MMIO_UART_TX_DATA;
// clang-format on

void tkey_writebyte(uint8_t b)
{
	for (;;) {
		if (*can_tx) {
			*tx = b;
			return;
		}
	}
}

void tkey_write(const uint8_t *buf, size_t nbytes)
{
	for (int i = 0; i < nbytes; i++) {
		tkey_writebyte(buf[i]);
	}
}

void tkey_write_with_header(const uint8_t *buf, size_t nbytes)
{
	tkey_writebyte(MODE_TKEYCTRL);
	tkey_writebyte(nbytes);

	for (int i = 0; i < nbytes; i++) {
		tkey_writebyte(buf[i]);
	}
}

void tkey_putchar(const uint8_t ch)
{
	tkey_writebyte(MODE_TKEYCTRL);
	tkey_writebyte(1);

	tkey_writebyte(ch);
}

void tkey_lf()
{
	tkey_writebyte(MODE_TKEYCTRL);
	tkey_writebyte(1);

	tkey_writebyte('\n');
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

void tkey_puthex(const uint8_t ch)
{
	tkey_writebyte(MODE_TKEYCTRL);
	tkey_writebyte(2);

	tkey_writebyte(hexnibble(ch >> 4 & 0x0f));
	tkey_writebyte(hexnibble(ch & 0x0f));
}

void tkey_putinthex(const uint32_t n)
{
	uint8_t buf[4];

	tkey_writebyte(MODE_TKEYCTRL);
	tkey_writebyte(2);

	memcpy(buf, &n, 4);
	tkey_write((uint8_t *)"0x", 2);
	for (int i = 3; i > -1; i--) {
		tkey_puthex(buf[i]);
	}
}

void tkey_puts(const char *s)
{
	size_t remaining = strlen(s);

	while (remaining > 0) {
		uint8_t len = (remaining < (PACKET_SIZE - HEADER_SIZE))
				  ? remaining
				  : (PACKET_SIZE - HEADER_SIZE);

		// Send chunk data
		tkey_write_with_header((const uint8_t *)s, len);

		s += len;
		remaining -= len;
	}
}

void tkey_hexdump(const uint8_t *buf, int len)
{
	uint8_t *byte_buf = (uint8_t *)buf;

	for (int i = 0; i < len; i++) {
		tkey_puthex(byte_buf[i]);
		if (i % 2 == 1) {
			tkey_putchar(' ');
		}

		if ((i + 1) % 16 == 0) {
			tkey_lf();
		}
	}

	tkey_lf();
}
