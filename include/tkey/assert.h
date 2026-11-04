// SPDX-FileCopyrightText: 2022 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#ifndef TKEY_ASSERT_H
#define TKEY_ASSERT_H

#include <tkey/io.h>

static inline uintptr_t assert_get_sp(void)
{
	uintptr_t sp;
	asm volatile("mv %0, sp" : "=r"(sp));
	return sp;
}

#if defined(QEMU_DEBUG)
#define assert(expr)                                                           \
	((expr) ? (void)(0)                                                    \
		: assert_fail(IO_QEMU, #expr, __FILE__, __LINE__, __func__,    \
			      assert_get_sp()))

#elif defined(TKEY_DEBUG)

#define assert(expr)                                                           \
	((expr) ? (void)(0)                                                    \
		: assert_fail(IO_DEBUG, #expr, __FILE__, __LINE__, __func__,   \
			      assert_get_sp()))

#else

#define assert(expr) ((expr) ? (void)(0) : assert_halt(assert_get_sp()))
#endif

void assert_fail(enum ioend dest, const char *assertion, const char *file,
		 unsigned int line, const char *function, uintptr_t sp)
    __attribute__((noinline, noreturn));
void assert_halt(uintptr_t sp) __attribute__((noinline, noreturn));
#endif
