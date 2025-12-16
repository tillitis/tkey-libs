// SPDX-FileCopyrightText: 2022 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include "tkey/platform.h"
#include <stddef.h>
#include <stdint.h>
#include <tkey/assert.h>
#include <tkey/debug.h>
#include <tkey/proto.h>
#include <tkey/tk1_mem.h>

// clang-format off
static volatile uint32_t *ver		= (volatile uint32_t *) TK1_MMIO_TK1_VERSION;
// clang-format on

// lentobytes returns the number of bytes a cmdlen takes. Returns non-zero on
// success.
static size_t lentobyte(enum cmdlen cmdlen)
{
	int len = 0;

	switch (cmdlen) {
	case LEN_1:
		len = 1;
		break;

	case LEN_4:
		len = 4;
		break;

	case LEN_32:
		len = 32;
		break;

	case LEN_128:
		len = 128;
		break;

	default:
		// Shouldn't happen
		return 0;
	}

	return len;
}

static int bytetolen(size_t nbytes, enum cmdlen *len)
{

	switch (nbytes) {
	case 1:
		*len = LEN_1;
		break;

	case 4:
		*len = LEN_4;
		break;

	case 32:
		*len = LEN_32;
		break;

	case 128:
		*len = LEN_128;
		break;

	default:
		// Shouldn't happen
		return -1;
	}

	return 0;
}

int genhdr(uint8_t id, uint8_t endpoint, uint8_t status, size_t nbytes,
	   uint8_t *header)
{
	*header |= (id << 5) & 0x60;
	*header |= (endpoint << 3) & 0x18;
	*header |= (status << 2) & 0x4;
	enum cmdlen len = 0;
	if (bytetolen(nbytes, &len)) {
		return -1;
	}
	*header |= len & 0x3;

	return 0;
}

int parseframe(uint8_t b, struct frame_header *hdr)
{
	if ((b & 0x80) != 0) {
		// Bad version
		return -1;
	}

	if ((b & 0x4) != 0) {
		// Must be 0
		return -1;
	}

	hdr->id = (b & 0x60) >> 5;
	hdr->endpoint = (b & 0x18) >> 3;
	hdr->len = lentobyte(b & 0x3);

	return 0;
}

// read_frame reads an entire frame according to the "framing protocol". Handles
// both Castor and Bellatrix. Uses the CDC endpoint.
//
// The framing header, wit the frame length, will be returned in hdr and the
// data in buf.
//
// The allocated size of buf, bufsize, needs to be equal or greater than
// the largest expected frame (not counting the framing header). If a header is
// received indicating a larger frame than bufsize, an error will be returned
// without reading the data. It is recommended to have a bufsize of 128 bytes,
// the maximum frame size according to the framing protocol.
//
// Does not handle interleaved frames from different endpoints, and
// hence should only be used with IO_CDC set via config_endpoints() (default).
//
// Returns zero on success.
int read_frame(uint8_t *buf, size_t bufsize, struct frame_header *hdr)
{
	uint8_t in = 0;

	if (*ver >= TKEY_VERSION_CASTOR) {
		if (serial_read(&in, 1, 1) != 0) {
			return -1;
		}
	} else {
		if (uart_read(&in, 1, 1) != 0) {
			return -1;
		}
	}

	if (parseframe(in, hdr) == -1) {
		return -1;
	}

	if (*ver >= TKEY_VERSION_CASTOR) {
		if (serial_read(buf, bufsize, hdr->len) != 0) {
			return -1;
		}
	} else {
		if (uart_read(buf, bufsize, hdr->len) != 0) {
			return -1;
		}
	}

	return 0;
}

// write_frame writes an entire frame according to the "framing protocol".
// Handles both Castor and Bellatrix. Uses the CDC endpoint.
//
// The frame size, nbytes, needs to be one of the defined lengths for a frame.
// See cmdlen in proto.h.
//
// Returns zero on success.
int write_frame(uint8_t status, uint8_t id, enum endpoints endpoint,
		uint8_t *buf, size_t nbytes)
{
	if (buf == NULL) {
		return -1;
	}

	if (nbytes == 0 || nbytes > CMDLEN_MAXBYTES) {
		return -1;
	}

	// Frame Protocol Header
	uint8_t header = 0;
	if (genhdr(id, endpoint, status, nbytes, &header)) {
		return -1;
	}
	enum ioend dst = IO_CDC;
	if (*ver < TKEY_VERSION_CASTOR) {
		dst = IO_UART;
	}

	write(dst, &header, sizeof(header));
	write(dst, buf, nbytes);
	return 0;
}
