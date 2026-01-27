// SPDX-FileCopyrightText: 2022 Tillitis AB <tillitis.se>
// SPDX-FileCopyrightText: 2025 Glasklar Teknik AB <glasklarteknik.se>
// SPDX-License-Identifier: BSD-2-Clause

#ifndef STRING_H
#define STRING_H

#include <stddef.h>

void *memchr(const void *p, int c, size_t n);
int memcmp(const void *a, const void *b, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *dest, int c, size_t n);
char *strchr(const char *s, int c);
int strcmp(const char *a, const char *b);
char *strcpy(char *dst, const char *src);
size_t strcspn(const char *s, const char *reject);
size_t strlen(const char *str);
int strncmp(const char *a, const char *b, size_t n);
size_t strspn(const char *s, const char *accept);

#endif /* STRING_H */
