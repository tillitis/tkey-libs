// SPDX-FileCopyrightText: 2026 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

// Bit banged UART, transmit one byte
//
// 8N1 at 500 kbaud. Least significant bit first.
// Assumes a 24 MHz clock frequency.
//
// Uses GPIO3
//
// Args:
//   a0 (x10): byte to transmit
	.globl bbuart_tx_byte

	.section ".text"

bbuart_tx_byte:
	c.li a1, 11			// Bit counter:
					//   1 start bit
					//   + x data bits
					//   + y stop bits
					//   + 1 idle state
	li a2, 0xff000028		// GPIO register address
	c.lw a3, 0(a2)			// GPIO register value
	c.li a5, 0xfffffffb		// GPIO pin mask

	c.slli a0, 1			// Add start bit
	ori a0, a0, 0b11000000000	// Add stop bit and idle state

	.balign 4			// Align to 32-bit boundary to get
					// deterministic instruction timing.
next_bit:
	// Get next bit
	andi a4, a0, 1			// Get least significant bit
	c.slli a4, 2			// Move bit to GPIO pin position

	// Update GPIO register
	c.and a3, a5			// Clear pin
	c.or a3, a4			// Copy bit to pin
	c.sw a3, 0(a2)			// Update GPIO register

	// Shift to next bit
	c.srli a0, 1

	// Do a NOP-like instuction that wastes the exact amount of cycles
	// needed to hit the target baudrate.
	c.bnez a1, delay		// a1 is always > 0 here. Jump to next
					// instruction.
delay:

	// Loop if bits left
	c.addi a1, -1
	c.bnez a1, next_bit

	ret
