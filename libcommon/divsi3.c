// SPDX-FileCopyrightText: 2026 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

static uint32_t udivmodsi4(uint32_t num, uint32_t den, uint32_t *rem_out)
{
	uint32_t quot = 0;
	uint32_t rem = 0;

	if (den == 0) {
		// Undefined in C. Many runtimes just return 0.
		if (rem_out) {
			*rem_out = 0;
		}
		return 0;
	}

	for (int i = 31; i >= 0; i--) {
		rem <<= 1;
		rem |= (num >> i) & 1u;

		if (rem >= den) {
			rem -= den;
			quot |= (1u << i);
		}
	}

	if (rem_out) {
		*rem_out = rem;
	}

	return quot;
}

int32_t __divsi3(int32_t a, int32_t b)
{
	// Division by zero
	if (b == 0) {
		assert(1 == 2);
	}

	if (a == (int32_t)0x80000000 && b == -1) {
		return (int32_t)0x80000000;
	}

	uint32_t ua = (a < 0) ? -(uint32_t)a : (uint32_t)a;
	uint32_t ub = (b < 0) ? -(uint32_t)b : (uint32_t)b;

	uint32_t uq = udivmodsi4(ua, ub, NULL);

	int neg = (a < 0) ^ (b < 0);

	uint32_t uresult = neg ? -uq : uq;

	return (int32_t)uresult;
}
