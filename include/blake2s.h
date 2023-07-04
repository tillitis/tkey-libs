// Copyright (C) Tillitis AB
// SPDX-License-Identifier: GPL-2.0-only

#ifndef BLAKE2S_H
#define BLAKE2S_H

#include <types.h>

// blake2s state context
typedef struct {
	uint8_t b[64]; // input buffer
	uint32_t h[8]; // chained state
	uint32_t t[2]; // total number of bytes
	size_t c;      // pointer for b[]
	size_t outlen; // digest size
} blake2s_ctx;

int blake2s(void *out, unsigned long outlen, const void *key,
	    unsigned long keylen, const void *in, unsigned long inlen,
	    blake2s_ctx *ctx);
#endif
