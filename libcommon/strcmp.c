// SPDX-FileCopyrightText: 2025 Glasklar Teknik AB <glasklarteknik.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <string.h>

int strcmp(const char *a, const char *b)
{
	while (*a && (*a == *b)) {
		a++;
		b++;
	}

	return *(unsigned char *)a - *(unsigned char *)b;
}
