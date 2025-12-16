// SPDX-FileCopyrightText: 2023 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <tkey/assert.h>
#include <tkey/debug.h>
#include <tkey/io.h>
#include <tkey/led.h>
#include <tkey/tk1_mem.h>

#define SLEEPTIME 100000

void sleep(uint32_t n)
{
	for (volatile int i = 0; i < n; i++)
		;
}

int main(void)
{
	debug_puts("Hello, world!\n");
	debug_puts("Going to sleep between blinks: ");
	debug_putinthex(SLEEPTIME);
	debug_lf();

	uint8_t buf[128] = {0x00};
	enum ioend endpoint = IO_NONE;
	uint8_t len;

	for (;;) {
		if (readselect(IO_CDC, true, &endpoint, &len)) {
			assert(1 == 2);
		}

		if (len > 0) {
			int nbytes = read(IO_CDC, buf, 128, len);
			write(IO_CDC, buf, nbytes);
		}

		led_set(LED_RED);
		sleep(SLEEPTIME);
		led_set(LED_GREEN);
		sleep(SLEEPTIME);
		led_set(LED_BLUE);
		sleep(SLEEPTIME);
	}
}
