// SPDX-FileCopyrightText: 2026 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <string.h>

size_t strspn(const char *s, const char *accept)
{
	const char *p;
	size_t n = 0;

	while (*s) {
		for (p = accept; *p; ++p) {
			if (*p == *s) {
				break;
			}
		}
		if (!*p) {
			break;
		}
		++s;
		++n;
	}
	return n;
}
