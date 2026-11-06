// SPDX-FileCopyrightText: 2022 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <tkey/assert.h>
#include <tkey/io.h>
#include <tkey/lib.h>
#include <tkey/syscall.h>
#include <tkey/tk1_mem.h>

void assert_fail(enum ioend dest, const char *assertion, const char *file,
		 unsigned int line, const char *function, uintptr_t sp)
{
	puts(dest, "assert: ");
	puts(dest, assertion);
	puts(dest, " ");
	puts(dest, file);
	puts(dest, ":");
	putinthex(dest, line);
	puts(dest, " ");
	puts(dest, function);
	puts(dest, "\n");

	if (sp >= TK1_RAM_BASE) {
		// Wipe the app stack
		asm volatile("la a0, _ebss;"
			     "la a1, _estack;"
			     "loop_app_assert_fail:;"
			     "sw zero, 0(a0);"
			     "addi a0, a0, 4;"
			     "blt a0, a1, loop_app_assert_fail;" ::
				 : "a0", "a1", "memory");
	}

	// Make syscall "ASSERT_FAIL"
	asm volatile("li a0, %0\n"
		     "li t1, 0xe1000000\n"
		     "sw zero, 0(t1)\n"
		     :
		     : "i"(TK1_SYSCALL_ASSERT_FAIL)
		     : "a0", "t1", "memory");

	// Not reached
	__builtin_unreachable();
}

void assert_halt(uintptr_t sp)
{
	if (sp >= TK1_RAM_BASE) {
		// Wipe the app stack
		asm volatile("la a0, _ebss;"
			     "la a1, _estack;"
			     "loop_app_assert_halt:;"
			     "sw zero, 0(a0);"
			     "addi a0, a0, 4;"
			     "blt a0, a1, loop_app_assert_halt;" ::
				 : "a0", "a1", "memory");
	}

	// Make syscall "ASSERT_FAIL"
	asm volatile("li a0, %0\n"
		     "li t1, 0xe1000000\n"
		     "sw zero, 0(t1)\n"
		     :
		     : "i"(TK1_SYSCALL_ASSERT_FAIL)
		     : "a0", "t1", "memory");

	// Not reached
	__builtin_unreachable();
}
