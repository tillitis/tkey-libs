// SPDX-FileCopyrightText: 2022 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <string.h>

void *memset(void *dest, int c, size_t n)
{
	uint8_t *s = dest;

	for (; n; n--, s++) {
		*s = c;
	}

	return dest;
}
