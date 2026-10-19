// SPDX-FileCopyrightText: 2026 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <string.h>

size_t strcspn(const char *s, const char *reject)
{
	const char *p;
	size_t n = 0;

	while (*s) {
		for (p = reject; *p; ++p) {
			if (*p == *s) {
				return n;
			}
		}
		++s;
		++n;
	}
	return n;
}
