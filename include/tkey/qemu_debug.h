// Copyright (C) 2023 - Tillitis AB
// SPDX-License-Identifier: GPL-2.0-only

#ifndef QEMU_DEBUG_H
#define QEMU_DEBUG_H

#include <stdint.h>

#ifndef QEMU_DEBUG
#define qemu_putchar(ch)
#define qemu_lf()
#define qemu_putinthex(n)
#define qemu_puts(s)
#define qemu_puthex(ch)
#define qemu_hexdump(buf, len)
#else
int qemu_putchar(const uint8_t ch);
void qemu_lf();
void qemu_putinthex(const uint32_t n);
void qemu_puts(const char *s);
void qemu_puthex(const uint8_t ch);
void qemu_hexdump(const uint8_t *buf, int len);
#endif

#endif
