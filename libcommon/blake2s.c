// Copyright (C) Tillitis AB
// SPDX-License-Identifier: GPL-2.0-only

#include <blake2s.h>
#include <types.h>
#include <tk1_mem.h>

int blake2s(void *out, unsigned long outlen, const void *key,
	    unsigned long keylen, const void *in, unsigned long inlen,
	    blake2s_ctx *ctx)
{
	int (*fw_blake2s)(void *out, unsigned long outlen, const void *key,
			  unsigned long keylen, const void *in,
			  unsigned long inlen, blake2s_ctx *ctx);

	fw_blake2s =
	    (int (*)(void *, unsigned long, const void *, unsigned long,
		     const void *, unsigned long, blake2s_ctx *)) *
	    (volatile uint32_t *)TK1_MMIO_TK1_BLAKE2S;

	return fw_blake2s(out, outlen, key, keylen, in, inlen, ctx);
}
