// Copyright (C) Tillitis AB
// SPDX-License-Identifier: GPL-2.0-only

#include <blake2s.h>
#include <tk1_mem.h>
#include <types.h>

typedef int (*fw_blake2s_p)(void *out, unsigned long outlen, const void *key,
			    unsigned long keylen, const void *in,
			    unsigned long inlen, blake2s_ctx *ctx);

int blake2s(void *out, unsigned long outlen, const void *key,
	    unsigned long keylen, const void *in, unsigned long inlen,
	    blake2s_ctx *ctx)
{
	fw_blake2s_p const fw_blake2s =
	    (fw_blake2s_p) * (volatile uint32_t *)TK1_MMIO_TK1_BLAKE2S;

	return fw_blake2s(out, outlen, key, keylen, in, inlen, ctx);
}
