// SPDX-FileCopyrightText: 2022 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

#ifndef TKEY_ASSERT_H
#define TKEY_ASSERT_H

#define assert(expr)                                                           \
	((expr) ? (void)(0) : assert_fail(#expr, __FILE__, __LINE__, __func__))

void assert_fail(const char *assertion, const char *file, unsigned int line,
		 const char *function);

#endif
