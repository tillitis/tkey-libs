// SPDX-FileCopyrightText: 2022 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <string.h>

void *memcpy(void *dest, const void *src, size_t n)
{
	const uint8_t *src_byte = src;
	uint8_t *dest_byte = dest;

	for (size_t i = 0; i < n; i++) {
		dest_byte[i] = src_byte[i];
	}

	return dest;
}
