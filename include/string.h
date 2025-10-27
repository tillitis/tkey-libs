// SPDX-FileCopyrightText: 2022 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#ifndef TKEY_STRING_H
#define TKEY_STRING_H

#include <stddef.h>

size_t strlen(const char *str);
void *memset(void *dest, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);

#endif /* TKEY_STRING_H */
