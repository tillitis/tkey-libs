// SPDX-FileCopyrightText: 2023 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#ifndef TKEY_BLAKE2S_H
#define TKEY_BLAKE2S_H

#include <stddef.h>
#include <stdint.h>

int blake2s(void *out, unsigned long outlen, const void *key,
	    unsigned long keylen, const void *in, unsigned long inlen);

#endif
