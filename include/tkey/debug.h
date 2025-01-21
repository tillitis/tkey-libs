// SPDX-FileCopyrightText: 2023 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#ifndef TKEY_DEBUG_H
#define TKEY_DEBUG_H

#include <stdint.h>

#if defined(QEMU_DEBUG)

void qemu_putchar(const uint8_t ch);
void qemu_lf();
void qemu_putinthex(const uint32_t n);
void qemu_puts(const char *s);
void qemu_puthex(const uint8_t ch);
void qemu_hexdump(const uint8_t *buf, int len);

#define debug_putchar qemu_putchar
#define debug_lf qemu_lf
#define debug_putinthex qemu_putinthex
#define debug_puts qemu_puts
#define debug_puthex qemu_puthex
#define debug_hexdump qemu_hexdump

#elif defined(TKEY_DEBUG)

void tkey_putchar(const uint8_t ch);
void tkey_lf();
void tkey_putinthex(const uint32_t n);
void tkey_puts(const char *s);
void tkey_puthex(const uint8_t ch);
void tkey_hexdump(const uint8_t *buf, int len);

#define debug_putchar tkey_putchar
#define debug_lf tkey_lf
#define debug_putinthex tkey_putinthex
#define debug_puts tkey_puts
#define debug_puthex tkey_puthex
#define debug_hexdump tkey_hexdump

#else

#define debug_putchar(ch)
#define debug_lf()
#define debug_putinthex(n)
#define debug_puts(s)
#define debug_puthex(ch)
#define debug_hexdump(buf, len)

#endif

#endif
