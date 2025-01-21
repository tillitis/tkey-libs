// SPDX-FileCopyrightText: 2022 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <tkey/assert.h>
#include <tkey/debug.h>
#include <tkey/lib.h>

void assert_fail(const char *assertion, const char *file, unsigned int line,
		 const char *function)
{
	debug_puts("assert: ");
	debug_puts(assertion);
	debug_puts(" ");
	debug_puts(file);
	debug_puts(":");
	debug_putinthex(line);
	debug_puts(" ");
	debug_puts(function);
	debug_lf();

	// Force illegal instruction to halt CPU
	asm volatile("unimp");

	// Not reached
	__builtin_unreachable();
}
