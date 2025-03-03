// SPDX-FileCopyrightText: 2025 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <tkey/assert.h>
#include <tkey/debug.h>
#include <tkey/lib.h>
#include <tkey/proto.h>
#include <tkey/tk1_mem.h>

// USB Mode Protocol:
//   1 byte mode
//   1 byte length
//
// Our USB Mode Protocol packets has room for 255 bytes according to
// the header but we use a packet size of 62 so we limit transfers to
// 64 bytes (2 byte header + 62 byte data) to fit in a single USB
// frame.
#define USBMODE_PACKET_SIZE 64

static void hex(uint8_t buf[2], const uint8_t c);
static int discard(size_t nbytes);
static uint8_t readbyte(void);
static void writebyte(uint8_t b);

struct usb_mode {
	enum ioend endpoint; // Current USB endpoint with data
	uint8_t len;	     // Data available in from current USB mode.
};

static struct usb_mode cur_endpoint = {
    IO_NONE,
    0,
};

// clang-format off
static volatile uint32_t* const can_rx = (volatile uint32_t *)TK1_MMIO_UART_RX_STATUS;
static volatile uint32_t* const rx =     (volatile uint32_t *)TK1_MMIO_UART_RX_DATA;
static volatile uint32_t* const can_tx = (volatile uint32_t *)TK1_MMIO_UART_TX_STATUS;
static volatile uint32_t* const tx =     (volatile uint32_t *)TK1_MMIO_UART_TX_DATA;
static volatile uint8_t* const debugtx = (volatile uint8_t *)TK1_MMIO_QEMU_DEBUG;
// clang-format on

// writebyte blockingly writes byte b to UART
static void writebyte(uint8_t b)
{
	for (;;) {
		if (*can_tx) {
			*tx = b;
			return;
		}
	}
}

// write_with_header writes nbytes of buf to UART with a USB Mode
// Protocol header telling the receiver about the mode and length.
static void write_with_header(enum ioend dest, const uint8_t *buf,
			      size_t nbytes)
{
	// USB Mode Protocol header:
	//   1 byte mode
	//   1 byte length

	writebyte(dest);
	writebyte(nbytes);

	for (int i = 0; i < nbytes; i++) {
		writebyte(buf[i]);
	}
}

// write blockingly writes nbytes bytes of data from buf to dest which
// is either:
//
// - IO_QEMU: QEMU debug port
// - IO_CDC: Through the UART for the CDC endpoint.
// - IO_HID: Through the UART for the HID endpoint.
// - IO_TKEYCTRL: Through the UART for the debug HID endpoint.
void write(enum ioend dest, const uint8_t *buf, size_t nbytes)
{
	if (dest == IO_QEMU) {
		for (int i = 0; i < nbytes; i++) {
			*debugtx = buf[i];
		}

		return;
	}

	while (nbytes > 0) {
		// We split the data into chunks that will fit in the
		// USB Mode Protocol and fits neatly in the USB frames
		// on the other side of the USB controller.
		uint8_t len =
		    nbytes < USBMODE_PACKET_SIZE ? nbytes : USBMODE_PACKET_SIZE;

		write_with_header(dest, (const uint8_t *)buf, len);

		buf += len;
		nbytes -= len;
	}
}

// readbyte reads a byte from UART and returns it. Blocking.
static uint8_t readbyte(void)
{
	for (;;) {
		if (*can_rx) {
			return *rx;
		}
	}

	return 0;
}

// read reads into buf of size bufsize from UART, nbytes or less, from
// the current USB endpoint. It doesn't block.
//
// Returns the number of bytes read. Empty data returns 0.
int read(enum ioend src, uint8_t *buf, size_t bufsize, size_t nbytes)
{
	if (buf == NULL || nbytes > bufsize) {
		return -1;
	}

	if (src != cur_endpoint.endpoint) {
		// No data for this source available right now.
		return 0;
	}

	int n = 0;

	for (n = 0; n < nbytes; n++) {
		buf[n] = readbyte();
		cur_endpoint.len--;
	}

	return n;
}

// discard nbytes of what's available.
//
// Returns how many bytes were discarded.
static int discard(size_t nbytes)
{
	int n = 0;
	uint8_t len = nbytes < cur_endpoint.len ? nbytes : cur_endpoint.len;

	for (n = 0; n < len; n++) {
		(void)readbyte();
		cur_endpoint.len--;
	}

	return nbytes;
}

// select blocks and returns when there is something readable from
// some mode.
//
// Use like this:
//
// endpoint = select(IO_CDC|IO_HID, &len)
//
// to wait for some data from either of the CDC or HID endpoint.
//
// Returns the first endpoint in the bitmask with data available.
// Indicates how many bytes available in len.
enum ioend readselect(int bitmask, uint8_t *len)
{
	for (;;) {
		// Check what is in the current UART buffer.
		//
		// - If nothing known, block until something comes along.
		//
		// - If not in bitmask, discard the data available
		//   from that endpoint.
		//
		// - If in the bitmask, return the first endpoint with
		//   data available and indicate how much data in len.
		if (cur_endpoint.len == 0) {
			// Read USB Mode Protocol header:
			//   1 byte mode
			//   1 byte length
			cur_endpoint.endpoint = readbyte();
			cur_endpoint.len = readbyte();
		}

		*len = cur_endpoint.len;

		if ((cur_endpoint.endpoint & bitmask) ==
		    cur_endpoint.endpoint) {
			*len = cur_endpoint.len;

			return cur_endpoint.endpoint;
		}

		// Not the USB endpoint caller asked for. Discard the
		// rest from this endpoint.
		discard(*len);
	}
}

void putchar(enum ioend dest, const uint8_t ch)
{
	write(dest, &ch, 1);
}

static void hex(uint8_t buf[2], const uint8_t c)
{
	unsigned int upper = (c >> 4) & 0xf;
	unsigned int lower = c & 0xf;

	buf[0] = upper < 10 ? '0' + upper : 'a' - 10 + upper;
	buf[1] = lower < 10 ? '0' + lower : 'a' - 10 + lower;
}

void puthex(enum ioend dest, const uint8_t c)
{
	uint8_t hexbuf[2];

	hex(hexbuf, c);
	write(dest, hexbuf, 2);
}

void putinthex(enum ioend dest, const uint32_t n)
{
	uint8_t buf[10];
	uint8_t hexbuf[2];
	uint8_t *intbuf = (uint8_t *)&n;

	buf[0] = '0';
	buf[1] = 'x';

	int j = 2;
	for (int i = 3; i > -1; i--) {
		hex(hexbuf, intbuf[i]);
		memcpy_s(&buf[j], 10 - j, hexbuf, 2);
		j += 2;
	}

	write(dest, buf, 10);
}

void puts(enum ioend dest, const char *s)
{
	write(dest, (const uint8_t *)s, strlen(s));
}

// Size of a hex row: Contains 16 bytes where each byte is printed as
// 3 characters (hex + hex + space). Every row ends with newline or at
// most CR+LF.
#define FULLROW (16 * 3)
#define ROWBUFSIZE (FULLROW + 2)

void hexdump(enum ioend dest, void *buf, int len)
{
	uint8_t rowbuf[ROWBUFSIZE] = {0};
	uint8_t hexbuf[2];
	uint8_t *byte_buf = (uint8_t *)buf;

	int rowpos = 0;
	for (int i = 0; i < len; i++) {
		hex(hexbuf, byte_buf[i]);
		rowbuf[rowpos++] = hexbuf[0];
		rowbuf[rowpos++] = hexbuf[1];
		rowbuf[rowpos++] = ' ';

		// If the row is full, print it now.
		if (rowpos == FULLROW) {
			if (dest == IO_CDC) {
				rowbuf[rowpos++] = '\r';
			}
			rowbuf[rowpos++] = '\n';
			write(dest, rowbuf, rowpos);
			rowpos = 0;
		}
	}

	// If final row wasn't full, print it now.
	if (rowpos != 0) {
		if (dest == IO_CDC) {
			rowbuf[rowpos++] = '\r';
		}
		rowbuf[rowpos++] = '\n';
		write(dest, rowbuf, rowpos);
	}
}
