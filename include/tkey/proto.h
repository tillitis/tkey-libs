// SPDX-FileCopyrightText: 2022 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <stddef.h>
#include <stdint.h>

#ifndef TKEY_PROTO_H
#define TKEY_PROTO_H

enum mode {
	MODE_QEMU = 0x10,
	MODE_TKEYCTRL = 0x20,
	MODE_CDC = 0x40,
	MODE_HID = 0x80,
};

enum endpoints {
	DST_HW_IFPGA = 0x00,
	DST_HW_AFPGA = 0x01,
	DST_FW = 0x02,
	DST_SW = 0x03
};

enum cmdlen {
	LEN_1,
	LEN_4,
	LEN_32,
	LEN_128
};

#define CMDLEN_MAXBYTES 128

enum status {
	STATUS_OK,
	STATUS_BAD
};

struct frame_header {
	uint8_t id;
	enum endpoints endpoint;
	size_t len;
};

uint8_t genhdr(uint8_t id, uint8_t endpoint, uint8_t status, enum cmdlen len);
int parseframe(uint8_t b, struct frame_header *hdr);
void write(const uint8_t *buf, size_t nbytes, enum mode mode);
int read(uint8_t *buf, size_t bufsize, size_t nbytes, enum mode expect_mode);
#endif
