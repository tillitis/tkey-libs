// SPDX-FileCopyrightText: 2026 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>

uint64_t __ashldi3(uint64_t u, int b)
{
	if ((unsigned)b >= 64) {
		return 0;
	}

	if (b == 0) {
		return u;
	}

	uint32_t lo = (uint32_t)(u);
	uint32_t hi = (uint32_t)(u >> 32);

	if (b >= 32) {
		hi = lo << (b - 32);
		lo = 0;
	} else {
		hi = (hi << b) | (lo >> (32 - b));
		lo = lo << b;
	}

	return ((uint64_t)hi << 32) | lo;
}
