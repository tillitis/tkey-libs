/*
 * Copyright (C) 2022, 2023 - Tillitis AB
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include <assert.h>
#include <lib.h>

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
}
