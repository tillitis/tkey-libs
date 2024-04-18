// Copyright (C) Tillitis AB
// SPDX-License-Identifier: GPL-2.0-only

#include <stdint.h>
#include <tkey/blake2s.h>
#include <tkey/tk1_mem.h>

// blake2s state context
typedef struct {
	uint8_t b[64]; // input buffer
	uint32_t h[8]; // chained state
	uint32_t t[2]; // total number of bytes
	size_t c;      // pointer for b[]
	size_t outlen; // digest size
} blake2s_ctx;

typedef int (*fw_blake2s_p)(void *out, unsigned long outlen, const void *key,
			    unsigned long keylen, const void *in,
			    unsigned long inlen, blake2s_ctx *ctx);

int blake2s(void *out, unsigned long outlen, const void *key,
	    unsigned long keylen, const void *in, unsigned long inlen)
{
	fw_blake2s_p const fw_blake2s =
	    (fw_blake2s_p) * (volatile uint32_t *)TK1_MMIO_TK1_BLAKE2S;
	blake2s_ctx ctx;
	return fw_blake2s(out, outlen, key, keylen, in, inlen, &ctx);
}
