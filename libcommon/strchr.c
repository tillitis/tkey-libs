// SPDX-FileCopyrightText: 2026 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <string.h>

char *strchr(const char *s, int c)
{
	for (; *s != (char)c; s++) {
		if (!*s) {
			return NULL;
		}
	}
	return (char *)s;
}
