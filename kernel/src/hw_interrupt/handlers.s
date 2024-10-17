# SPDX-License-Identifier: MPL-2.0

.set noreorder
.text

.extern _save_ee_frame
.extern _load_ee_frame

.extern __excep_interrupt_handler
.globl __excep_return
.globl __excep_death

__excep_death:
li $a0, 0xDEADB00B
nop
b __excep_death
nop

.macro mac_save_frame
	la $k0, __excep_interrupt_handler_frame
	sq $s0, 0x100($k0)
	sq $zero, 0x1A0($k0) # frames don't save k0
	sq $sp, 0x1D0($k0)
	sq $ra, 0x1F0($k0)

	# Change the stack size in interrupt.c if you change this
	la $sp, __excep_interrupt_stack + 8192
	move $s0, $k0
	jal _save_ee_frame
	nop
.endm

.macro mac_load_frame
	jal _load_ee_frame
	nop
	lq $ra, 0x1F0($s0)
	lq $s0, 0x100($s0)
.endm

.macro mac_exit_vector
	sync.l
	sync.p
	eret
	nop # Bad things have happened if we end up here !
	j __excep_death
	li $a1, 0x00000005
.endm

.section .vec.tlb, "ax", @progbits
__vec_excep_tlb:
j __excep_death
li $a1, 0x00000004

.section .vec.perfc, "ax", @progbits
__vec_excep_perf:
j __excep_death
li $a1, 0x00000003

.section .vec.debug, "ax", @progbits
__vec_excep_debug:
j __excep_death
li $a1, 0x00000002

.section .vec.common, "ax", @progbits
__vec_excep_common:
mac_save_frame
jal _vec_c_common
nop
mac_load_frame
mac_exit_vector

.section .vec.interrupt, "ax", @progbits
__vec_excep_interrupt:
j __excep_interrupt
nop

.section .text
__excep_interrupt:
mac_save_frame
jal _vec_c_interrupt
nop
mac_load_frame
mac_exit_vector
