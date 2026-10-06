// SPDX-FileCopyrightText: 2022 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>
#include <tkey/assert.h>
#include <tkey/io.h>
#include <tkey/lib.h>
#include <tkey/tk1_mem.h>

#define TK1_MMIO_RESETINFO_BASE 0xd0000f00
#define TK1_MMIO_RESETINFO_SIZE 0x100

#define CDI_SIZE 32
#define RESET_DIGEST_SIZE 32
#define RESET_DATA_SIZE 184

// clang-format off
static volatile uint32_t *cdi           = (volatile uint32_t *)TK1_MMIO_TK1_CDI_FIRST;
static volatile struct reset *resetinfo = (volatile struct reset *)TK1_MMIO_RESETINFO_BASE;
// clang-format on

enum reset_start {
	START_FLASH0 = 0,
	START_FLASH1_VER = 1,
	START_CLIENT = 2,
	START_CLIENT_VER = 3,
};

struct reset {
	enum reset_start type;
	uint8_t mask;
	uint8_t app_digest[RESET_DIGEST_SIZE];
	uint8_t measured_id[RESET_DIGEST_SIZE];
	uint8_t next_app_data[RESET_DATA_SIZE];
} __attribute__((__packed__));

void assert_fail(enum ioend dest, const char *assertion, const char *file,
		 unsigned int line, const char *function)
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

	(void)memset((void *)cdi, 0, CDI_SIZE);
	(void)memset((void *)resetinfo->app_digest, 0, RESET_DIGEST_SIZE);
	(void)memset((void *)resetinfo->measured_id, 0, RESET_DIGEST_SIZE);
	(void)memset((void *)resetinfo->next_app_data, 0, RESET_DATA_SIZE);

	// Clear the stack
	asm volatile("la a0, _ebss;"
		     "la a1, _estack;"
		     "loopstackassertfail:;"
		     "sw zero, 0(a0);"
		     "addi a0, a0, 4;"
		     "blt a0, a1, loopstackassertfail;" ::
			 : "memory");

	// Force illegal instruction to halt CPU
	for (;;) {
		asm volatile("unimp");
		asm volatile("unimp");
		asm volatile("unimp");
		asm volatile("unimp");
	}

	// Not reached
	__builtin_unreachable();
}

void assert_halt(void)
{
	(void)memset((void *)cdi, 0, CDI_SIZE);
	(void)memset((void *)resetinfo->app_digest, 0, RESET_DIGEST_SIZE);
	(void)memset((void *)resetinfo->measured_id, 0, RESET_DIGEST_SIZE);
	(void)memset((void *)resetinfo->next_app_data, 0, RESET_DATA_SIZE);

	// Clear the stack
	asm volatile("la a0, _ebss;"
		     "la a1, _estack;"
		     "loopstackasserthalt:;"
		     "sw zero, 0(a0);"
		     "addi a0, a0, 4;"
		     "blt a0, a1, loopstackasserthalt;" ::
			 : "memory");

	// Force illegal instruction to halt CPU
	for (;;) {
		asm volatile("unimp");
		asm volatile("unimp");
		asm volatile("unimp");
		asm volatile("unimp");
	}

	// Not reached
	__builtin_unreachable();
}
