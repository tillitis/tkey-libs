/*
 * Copyright (C) 2022, 2023 - Tillitis AB
 * SPDX-License-Identifier: BSD-2-Clause
 * */

#include <tkey/assert.h>
#include <tkey/lib.h>
#include <tkey/qemu_debug.h>

void assert_fail(const char *assertion, const char *file, unsigned int line,
		 const char *function)
{
	qemu_puts("assert: ");
	qemu_puts(assertion);
	qemu_puts(" ");
	qemu_puts(file);
	qemu_puts(":");
	qemu_putinthex(line);
	qemu_puts(" ");
	qemu_puts(function);
	qemu_lf();

	// Force illegal instruction to halt CPU
	asm volatile("unimp");

	// Not reached
	__builtin_unreachable();
}
