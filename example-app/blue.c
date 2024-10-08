// Copyright (C) Tillitis AB
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <tkey/led.h>
#include <tkey/tk1_mem.h>
#include <tkey/qemu_debug.h>

#define SLEEPTIME 100000

void sleep(uint32_t n)
{
	for (volatile int i = 0; i < n; i++);
}

int main(void)
{
	qemu_puts("Hello, world!\n");
	qemu_puts("Going to sleep between blinks: ");
	qemu_putinthex(SLEEPTIME);
	qemu_lf();

	for (;;) {
		led_set(LED_RED);
		sleep(SLEEPTIME);
		led_set(LED_GREEN);
		sleep(SLEEPTIME);
		led_set(LED_BLUE);
		sleep(SLEEPTIME);
	}
}
