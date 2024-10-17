// SPDX-License-Identifier: MPL-2.0

#pragma once

// EE register frames for exception handlers
struct ee_frame_t
{
	qword_t zero, at, v0, v1, a0, a1, a2, a3, a4, a5, a6, a7, t0, t1, t2, t3, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra;
};

// This will have to be duplicated once L2 exceptions are supported
extern "C" {
// This size should be the same in frame.s
const u32 __excep_interrupt_stack_size = 8 * 1024;
extern ee_frame_t __excep_interrupt_handler_frame __attribute__((aligned(128)));
extern u128 __excep_interrupt_stack[__excep_interrupt_stack_size / 16] __attribute__((aligned(128)));
}
