# SPDX-License-Identifier: MPL-2.0
.text

.set noreorder

.globl _save_ee_frame
.globl _load_ee_frame

_save_ee_frame:
	sq $zero, 0x00($s0)
.set noat
	sq $at, 0x10($s0)
.set at
	sq $v0, 0x20($s0)
	sq $v1, 0x30($s0)
	sq $a0, 0x40($s0)
	sq $a1, 0x50($s0)
	sq $a2, 0x60($s0)
	sq $a3, 0x70($s0)
	sq $a4, 0x80($s0)
	sq $a5, 0x90($s0)
	sq $a6, 0xA0($s0)
	sq $a7, 0xB0($s0)
	sq $t0, 0xC0($s0)
	sq $t1, 0xD0($s0)
	sq $t2, 0xE0($s0)
	sq $t3, 0xF0($s0)
	# sq $zero, 0x100($s0)
	sq $s1, 0x110($s0)
	sq $s2, 0x120($s0)
	sq $s3, 0x130($s0)
	sq $s4, 0x140($s0)
	sq $s5, 0x150($s0)
	sq $s6, 0x160($s0)
	sq $s7, 0x170($s0)
	sq $t8, 0x180($s0)
	sq $t9, 0x190($s0)
	sq $k0, 0x1A0($s0)
	sq $k1, 0x1B0($s0)
	sq $gp, 0x1C0($s0)
	sq $sp, 0x1D0($s0)
	sq $fp, 0x1E0($s0)
	sq $ra, 0x1F0($s0)
	jr $ra
	nop

_load_ee_frame:
	lq $zero, 0x00($s0)
.set noat
	lq $at, 0x10($s0)
.set at
	lq $v0, 0x20($s0)
	lq $v1, 0x30($s0)
	lq $a0, 0x40($s0)
	lq $a1, 0x50($s0)
	lq $a2, 0x60($s0)
	lq $a3, 0x70($s0)
	lq $a4, 0x80($s0)
	lq $a5, 0x90($s0)
	lq $a6, 0xA0($s0)
	lq $a7, 0xB0($s0)
	lq $t0, 0xC0($s0)
	lq $t1, 0xD0($s0)
	lq $t2, 0xE0($s0)
	lq $t3, 0xF0($s0)
	# lq $s0, 0x100($s0)
	lq $s1, 0x110($s0)
	lq $s2, 0x120($s0)
	lq $s3, 0x130($s0)
	lq $s4, 0x140($s0)
	lq $s5, 0x150($s0)
	lq $s6, 0x160($s0)
	lq $s7, 0x170($s0)
	lq $t8, 0x180($s0)
	lq $t9, 0x190($s0)
	lq $k0, 0x1A0($s0)
	lq $k1, 0x1B0($s0)
	lq $gp, 0x1C0($s0)
	lq $sp, 0x1D0($s0)
	lq $fp, 0x1E0($s0)
	#lq $ra, 0x1F0($s0)
	jr $ra
	nop
