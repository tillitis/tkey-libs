// SPDX-FileCopyrightText: 2022 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <string.h>

size_t strlen(const char *str)
{
	const char *s;

	for (s = str; *s; ++s)
		;

	return (s - str);
}

void *memset(void *dest, int c, size_t n)
{
	uint8_t *s = dest;

	for (; n; n--, s++)
		*s = c;

	return dest;
}

void *memcpy(void *dest, const void *src, size_t n)
{
	const uint8_t *src_byte = src;
	uint8_t *dest_byte = dest;

	for (size_t i = 0; i < n; i++) {
		dest_byte[i] = src_byte[i];
	}

	return dest;
}
