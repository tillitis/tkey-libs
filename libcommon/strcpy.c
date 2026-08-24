// SPDX-FileCopyrightText: 2022 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <string.h>

char *strcpy(char *dst, const char *src)
{
	char *ret = dst;
	while ((*dst++ = *src++))
		;
	return ret;
}
