// Copyright (C) Tillitis AB
// SPDX-License-Identifier: GPL-2.0-only

#ifndef TKEY_BLAKE2S_H
#define TKEY_BLAKE2S_H

#include <stddef.h>
#include <stdint.h>

int blake2s(void *out, unsigned long outlen, const void *key,
	    unsigned long keylen, const void *in, unsigned long inlen);

#endif
