# SPDX-License-Identifier: MPL-2.0

	.set noreorder
	.globl __start
__start:
	jal __mask_irqs
	nop
	jal __init_reg
	nop
	jal __clearbss
	nop
	j kmain_bootstrap
	nop

__mask_irqs:
	# Mask IM7 (COP0 compare int)
	mfc0 $t0, $12
	li $t1, 0x8000
	not $t1
	and $t0, $t1
	mtc0 $t0, $12

	# I_STAT ack
	la $t0, 0x1000f000
	li $t1, 0xFFFF
	sw $t1, 0($t0)

	# I_MASK clear
	la $t0, 0x1000f010
	lw $t1, 0($t0)
	sw $t1, 0($t0)

	# DMAC STAT
	la $t0, 0x1000E010
	lui $t1, 0x63FF
	lw $t2, 0($t0)
	and $t2, $t1
	sw $t2, 0($t0)
	jr $ra
	nop

__init_reg:
	la $a0, __kern_stack_top
	move $sp, $a0
	move $fp, $a0
	la $gp, _gp
	jr $ra
	nop

__clearbss:
	la $v1, __bss_end
	la $v0, __bss_start
	beq $v0, $v1, exit
	nop
loop:
	sq $zero, 0($v0)
	addiu $v0, 0x10
	slt $a0, $v1, $v0
	beq $a0, $zero, loop
	nop
exit:
	jr $ra
