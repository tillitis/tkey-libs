// SPDX-FileCopyrightText: 2026 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>

// Bit banged UART, transmit one byte
//
// 8N1 at 500 kbaud. Least significant bit first.
// Assumes a 24 MHz clock frequency.
//
// Uses GPIO3
void bbuart_tx_byte(uint8_t byte);
