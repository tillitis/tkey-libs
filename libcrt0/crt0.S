// SPDX-FileCopyrightText: 2022 Tillitis AB <tillitis.se>
// SPDX-License-Identifier: BSD-2-Clause

	.section ".text.init"
	.global _start
_start:
/* Example to show that the capture is correct
	li x1, 1
	li x2, 2
	li x3, 3
	li x4, 4
	li x5, 5
	li x6, 6
	li x7, 7
	li x8, 8
	li x9, 9
	li x10,0xa
	li x11,0xb
	li x12,0xc
	li x13,0xd
	li x14,0xe
	li x15,0xf
	li x16,10
	li x17,11
	li x18,12
	li x19,13
	li x20,14
	li x21,15
	li x22,16
	li x23,17
	li x24,18
	li x25,19
	li x26,0x1a
	li x27,0x1b
	li x28,0x1c
	li x29,0x1d
	li x30,0x1e
	li x31,0x1f
*/
	/* init stack to _estack, defined by app.lds */
	la sp, _estack

	addi sp, sp, -128

	sw x0,    0(sp)
	sw x1,    4(sp)
	sw x2,    8(sp) // captures SP set above, but capture it anway
	sw x3,   12(sp)
	sw x4,   16(sp)
	sw x5,   20(sp)
	sw x6,   24(sp)
	sw x7,   28(sp)
	sw x8,   32(sp)
	sw x9,   36(sp)
	sw x10,  40(sp)
	sw x11,  44(sp)
	sw x12,  48(sp)
	sw x13,  52(sp)
	sw x14,  56(sp)
	sw x15,  60(sp)
	sw x16,  64(sp)
	sw x17,  68(sp)
	sw x18,  72(sp)
	sw x19,  76(sp)
	sw x20,  80(sp)
	sw x21,  84(sp)
	sw x22,  88(sp)
	sw x23,  92(sp)
	sw x24,  96(sp)
	sw x25, 100(sp)
	sw x26, 104(sp)
	sw x27, 108(sp)
	sw x28, 112(sp)
	sw x29, 116(sp)
	sw x30, 120(sp)
	sw x31, 124(sp)


	/* zero-init bss section */
	la a0, _sbss
	la a1, _ebss
	bge a0, a1, end_init_bss

loop_init_bss:
	sw zero, 0(a0)
	addi a0, a0, 4
	blt a0, a1, loop_init_bss

end_init_bss:
	call main
