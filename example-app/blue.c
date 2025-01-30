// SPDX-FileCopyrightText: 2023 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <tkey/led.h>
#include <tkey/tk1_mem.h>
#include <tkey/qemu_debug.h>
#include <tkey/syscall.h>

#define SLEEPTIME 100000

void sleep(uint32_t n)
{
	for (volatile int i = 0; i < n; i++);
}

int main(void)
{
	syscall_enable();

	qemu_puts("Hello, world!\n");
	qemu_puts("Going to sleep between blinks: ");
	qemu_putinthex(SLEEPTIME);
	qemu_lf();

	for (int i = 0; i < 2; i++) {
		led_set(LED_RED);
		sleep(SLEEPTIME);
		led_set(LED_GREEN);
		sleep(SLEEPTIME);
		led_set(LED_BLUE);
		sleep(SLEEPTIME);
	}

	syscall(TKEY_SYSCALL_SET_LED, LED_RED | LED_BLUE);

	sleep(10 * SLEEPTIME);

	syscall(TKEY_SYSCALL_RESET, 0);

	for (;;) {
	}
}
