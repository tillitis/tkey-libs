// SPDX-FileCopyrightText: 2025 Glasklar Teknik AB <glasklarteknik.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <string.h>

void *memchr(const void *p, int c, size_t n)
{
	const uint8_t *s = p;
	for (size_t i = 0; i < n; i++) {
		if (s[i] == (uint8_t)c) {
			return (void *)&s[i];
		}
	}
	return NULL;
}
