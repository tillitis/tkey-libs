// SPDX-FileCopyrightText: 2023 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include "tkey/assert.h"
#include <stddef.h>
#include <stdint.h>
#include <tkey/debug.h>
#include <tkey/led.h>
#include <tkey/tk1_mem.h>

#define SLEEPTIME 100000

void print_registers(const uint32_t *regs)
{
	for (size_t i = 0; i < 32; i++) {
		debug_puts("x");
		debug_putinthex(i);
		debug_puts(" = ");
		debug_putinthex(regs[i]);
		debug_lf();
	}
}

void sleep(uint32_t n)
{
	for (volatile int i = 0; i < n; i++);
}

extern unsigned char _estack[];

int main(void)
{
	config_endpoints(IO_DEBUG);
	// make sure it has enumerated
	sleep(4 * SLEEPTIME);

	debug_puts("Hello, world!\n");
	debug_puts("Going to sleep between blinks: ");
	debug_putinthex(SLEEPTIME);
	debug_lf();

	const uint32_t *p_regs = (const uint32_t *)(_estack - (32 * 4));

	print_registers(p_regs);

	for (;;) {
		led_set(LED_RED);
		sleep(SLEEPTIME);
		led_set(LED_GREEN);
		sleep(SLEEPTIME);
		led_set(LED_BLUE);
		sleep(SLEEPTIME);
	}
}
