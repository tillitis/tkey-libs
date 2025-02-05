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

void tkey_putchar(const uint8_t ch)
{
	write(&ch, 1, MODE_TKEYCTRL);
}

void tkey_lf()
{
	tkey_putchar('\n');
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

uint8_t *hex(const uint8_t ch)
{
	static uint8_t buf[2];

	buf[0] = hexnibble(ch >> 4 & 0x0f);
	buf[1] = hexnibble(ch & 0x0f);

	return buf;
}

void tkey_puthex(const uint8_t ch)
{
	write(hex(ch), 2, MODE_TKEYCTRL);
}

void tkey_putinthex(const uint32_t n)
{
	uint8_t buf[10];
	uint8_t *intbuf = (uint8_t *)&n;

	buf[0] = '0';
	buf[1] = 'x';

	int j = 2;
	for (int i = 3; i > -1; i--) {
		memcpy(&buf[j], hex(intbuf[i]), 2);
		j += 2;
	}

	write(buf, 10, MODE_TKEYCTRL);
}

void tkey_puts(const char *s)
{
	write((const uint8_t *)s, strlen(s), MODE_TKEYCTRL);
}

static void writehexrow(uint8_t *buf, size_t len)
{
	buf[len] = '\n';
	len++;
	write(buf, len, MODE_TKEYCTRL);
}

void tkey_hexdump(const uint8_t *buf, int len)
{
	uint8_t linebuf[16 * 3 + 1] = {
	    0};		       // 16 bytes printed 3 chars + final newline
	uint8_t *hexbuf = {0}; // A byte represented as 2 chars and a space

	int pos = 0;
	for (int i = 0; i < len;) {
		hexbuf = hex(buf[i++]);
		memcpy(&linebuf[pos], hexbuf, 2);
		pos += 2;
		linebuf[pos] = ' ';
		pos++;

		if (i % 16 == 0) {
			writehexrow(linebuf, pos);
			pos = 0;
		}
	}

	// Final row wasn't full, so print it anyway.
	if (pos != 0) {
		writehexrow(linebuf, pos);
	}
}
