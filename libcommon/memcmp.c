// SPDX-FileCopyrightText: 2025 Glasklar Teknik AB <glasklarteknik.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <string.h>

int memcmp(const void *a, const void *b, size_t n)
{
	const uint8_t *a_byte = a;
	const uint8_t *b_byte = b;
	size_t i;

	for (i = 0; i < n; i++) {
		int d = a_byte[i] - b_byte[i];
		if (d != 0) {
			return d;
		}
	}
	return 0;
}
