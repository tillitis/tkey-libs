/*
 * SPDX-FileCopyrightText: 2022 Tillitis AB <tillitis.se>
 * SPDX-License-Identifier: BSD-2-Clause
 */

OUTPUT_ARCH( "riscv" )
ENTRY(_start)

/* Define stack size */
STACK_SIZE = 16K;

MEMORY
{
	RAM (rwx)   : ORIGIN = 0x40000000, LENGTH = 0x20000 /* 128 KB */
}

SECTIONS
{
	.text.init :
	{
		*(.text.init)
	} >RAM

	.stack (NOLOAD) :
	{
		. = ALIGN(16);
		_sstack = .;
		. += STACK_SIZE;
		. = ALIGN(16);
		_estack = .;
		_sidata = _estack;
	} >RAM

	.text :
	{
		. = ALIGN(4);
		_stext = .;
		*(.text)           /* .text sections (code) */
		*(.text*)          /* .text* sections (code) */
		*(.rodata)         /* .rodata sections (constants, strings, etc.) */
		*(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
		*(.srodata)        /* .rodata sections (constants, strings, etc.) */
		*(.srodata*)       /* .rodata* sections (constants, strings, etc.) */
		. = ALIGN(4);
		_etext = .;
		_sidata = _etext;
	} >RAM

	.data : AT (_etext)
	{
		. = ALIGN(4);
		_sdata = .;
		. = ALIGN(4);
		*(.data)           /* .data sections */
		*(.data*)          /* .data* sections */
		*(.sdata)          /* .sdata sections */
		*(.sdata*)         /* .sdata* sections */
		. = ALIGN(4);
		_edata = .;
	} >RAM

	/* Uninitialized data section */
	.bss :
	{
		. = ALIGN(4);
		_sbss = .;
		*(.bss)
		*(.bss*)
		*(.sbss)
		*(.sbss*)
		*(COMMON)

		. = ALIGN(4);
		_ebss = .;
	} >RAM

}
