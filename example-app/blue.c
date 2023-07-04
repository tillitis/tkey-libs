// Copyright (C) Tillitis AB
// SPDX-License-Identifier: GPL-2.0-only

#include <types.h>
#include <tk1_mem.h>

#define SLEEPTIME 100000
#define LED_RED   (1 << TK1_MMIO_TK1_LED_R_BIT)
#define LED_GREEN (1 << TK1_MMIO_TK1_LED_G_BIT)
#define LED_BLUE  (1 << TK1_MMIO_TK1_LED_B_BIT)

static volatile uint32_t* const led = (volatile uint32_t *)TK1_MMIO_TK1_LED;

void sleep(uint32_t n)
{
	for (volatile int i = 0; i < n; i++);
}

int main(void)
{
	for (;;) {
		*led = LED_RED;
		sleep(SLEEPTIME);
		*led = LED_GREEN;
		sleep(SLEEPTIME);
		*led = LED_BLUE;
		sleep(SLEEPTIME);
	}
}
