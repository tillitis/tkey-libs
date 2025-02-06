// SPDX-FileCopyrightText: 2022 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <stddef.h>
#include <stdint.h>
#include <tkey/debug.h>
#include <tkey/proto.h>
#include <tkey/tk1_mem.h>

// USB Mode Protocol:
//   1 byte mode
//   1 byte length
//
// Our USB Mode Protocol packets has room for 255 bytes according to
// the header but we use a packet size of 64 which neatly fits in with
// the USB packet size from the CH552 to the client.
#define USBMODE_PACKET_SIZE 64

// clang-format off
static volatile uint32_t* const can_rx = (volatile uint32_t *)TK1_MMIO_UART_RX_STATUS;
static volatile uint32_t* const rx =     (volatile uint32_t *)TK1_MMIO_UART_RX_DATA;
static volatile uint32_t* const can_tx = (volatile uint32_t *)TK1_MMIO_UART_TX_STATUS;
static volatile uint32_t* const tx =     (volatile uint32_t *)TK1_MMIO_UART_TX_DATA;
// clang-format on

static void writebyte(uint8_t b);
static uint8_t readbyte(void);
static void write_with_header(const uint8_t *buf, size_t nbytes, enum mode mode);
static void qemu_write(const uint8_t *buf, size_t nbytes);

uint8_t genhdr(uint8_t id, uint8_t endpoint, uint8_t status, enum cmdlen len)
{
	return (id << 5) | (endpoint << 3) | (status << 2) | len;
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

	// Length
	switch (b & 0x3) {
	case LEN_1:
		hdr->len = 1;
		break;
	case LEN_4:
		hdr->len = 4;
		break;
	case LEN_32:
		hdr->len = 32;
		break;
	case LEN_128:
		hdr->len = 128;
		break;
	default:
		// Unknown length
		return -1;
	}

	return 0;
}

static void writebyte(uint8_t b)
{
	for (;;) {
		if (*can_tx) {
			*tx = b;
			return;
		}
	}
}

static void write_with_header(const uint8_t *buf, size_t nbytes, enum mode mode)
{
	// Append USB Mode Protocol header:
	//   1 byte mode
	//   1 byte length

	writebyte(mode);
	writebyte(nbytes);

	for (int i = 0; i < nbytes; i++) {
		writebyte(buf[i]);
	}
}

static void qemu_write(const uint8_t *buf, size_t nbytes)
{
	static volatile uint8_t *const debugtx =
	    (volatile uint8_t *)TK1_MMIO_QEMU_DEBUG;

	for (int i = 0; i < nbytes; i++) {
		*debugtx = buf[i];
	}
}

// write blockingly writes nbytes bytes of data from buf to the UART,
// framing the data in USB Mode Protocol with mode mode, either
// MODE_CDC or MODE_HID.
void write(const uint8_t *buf, size_t nbytes, enum mode mode)
{
	if (mode == MODE_QEMU) {
		qemu_write(buf, nbytes);

		return;
	}

	while (nbytes > 0) {
		// We split the data into chunks that will fit in the
		// USB Mode Protocol with some spare change.
		uint8_t len = (nbytes < (USBMODE_PACKET_SIZE))
				  ? nbytes
				  : (USBMODE_PACKET_SIZE);

		write_with_header((const uint8_t *)buf, len, mode);

		buf += len;
		nbytes -= len;
	}
}

static uint8_t readbyte(void)
{
	for (;;) {
		if (*can_rx) {
			return *rx;
		}
	}
}

// read blockingly reads nbytes bytes of data into buffer buf, a
// maximum bufsize bytes.
//
// Caller asks for the expected USB mode expect_mode: MODE_CDC or
// MODE_HID, which represents different endpoints on the USB
// controller.
//
// If data is readable but with another mode set, it is silently
// discarded and we keep on reading until nbytes bytes have appeared.
//
int read(uint8_t *buf, size_t bufsize, size_t nbytes, enum mode expect_mode)
{
	static uint8_t mode = 0;
	static uint8_t mode_bytes_left = 0;

	if (nbytes > bufsize) {
		return -1;
	}

	int n = 0;
	while (n < nbytes) {
		if (mode_bytes_left == 0) {
			// Read USB Mode Protocol header:
			//   1 byte mode
			//   1 byte length
			mode = readbyte();
			mode_bytes_left = readbyte();
		}

		if (mode == expect_mode) {
			// Reading payload.
			buf[n] = readbyte();
			n++;
			mode_bytes_left--;
		} else {
			// Not the USB mode caller asked for. Eat the rest.
			for (int i = 0; i < mode_bytes_left; i++) {
				(void)readbyte();
			}

			mode_bytes_left = 0;
		}
	}

	return 0;
}
