// SPDX-FileCopyrightText: 2024 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>

#ifndef TKEY_SYSCALL_H
#define TKEY_SYSCALL_H

void syscall_enable(void);
int syscall(uint32_t number, uint32_t arg1);

#define TKEY_SYSCALL_RESET 1
#define TKEY_SYSCALL_SET_LED 10

#endif
