// SPDX-FileCopyrightText: 2022 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <stddef.h>
#include <stdint.h>
#include <tkey/proto.h>
#include <tkey/qemu_debug.h>
#include <tkey/tk1_mem.h>

// clang-format off
static volatile uint32_t* const can_rx =          (volatile uint32_t *)TK1_MMIO_UART_RX_STATUS;
static volatile uint32_t* const rx =              (volatile uint32_t *)TK1_MMIO_UART_RX_DATA;
static volatile uint32_t* const can_tx =          (volatile uint32_t *)TK1_MMIO_UART_TX_STATUS;
static volatile uint32_t* const tx =              (volatile uint32_t *)TK1_MMIO_UART_TX_DATA;
static volatile uint32_t* const timer =           (volatile uint32_t *)TK1_MMIO_TIMER_TIMER;
static volatile uint32_t* const timer_prescaler = (volatile uint32_t *)TK1_MMIO_TIMER_PRESCALER;
static volatile uint32_t* const timer_status =    (volatile uint32_t *)TK1_MMIO_TIMER_STATUS;
static volatile uint32_t* const timer_ctrl =      (volatile uint32_t *)TK1_MMIO_TIMER_CTRL;
// clang-format on

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

void writebyte(uint8_t b)
{
	for (;;) {
		if (*can_tx) {
			*tx = b;
			return;
		}
	}
}

void write(const uint8_t *buf, size_t nbytes)
{
	for (int i = 0; i < nbytes; i++) {
		writebyte(buf[i]);
	}
}

uint8_t readbyte()
{
	for (;;) {
		if (*can_rx) {
			return *rx;
		}
	}
}

int read(uint8_t *buf, size_t bufsize, size_t nbytes)
{
	if (nbytes > bufsize) {
		return -1;
	}

	for (int n = 0; n < nbytes; n++) {
		buf[n] = readbyte();
	}

	return 0;
}

size_t read_timeout(uint8_t *buf, size_t nbytes, uint32_t timeout,
		    uint32_t prescaler)
{
	int n;

	*timer = timeout;
	*timer_prescaler = prescaler;

	// Start timer
	*timer_ctrl |= (1 << TK1_MMIO_TIMER_CTRL_START_BIT);
	for (n = 0; n < nbytes; n++) {

		for (;;) {
			if (*can_rx) {
				buf[n] = *rx;
				break;
			}

			if (*timer_status == 1) {
				// Timer expired
				return n;
			}
		}
	}

	// Stop and reset timer
	if (*timer_status != 1) {
		*timer_ctrl |= (1 << TK1_MMIO_TIMER_CTRL_START_BIT);
	}

	return n;
}
