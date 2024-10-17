// SPDX-License-Identifier: MPL-2.0

#include "hw_interrupt/common.hpp"

extern "C" {
ee_frame_t __excep_interrupt_handler_frame __attribute__((aligned(128)));
u128 __excep_interrupt_stack[__excep_interrupt_stack_size / 16] __attribute__((aligned(128)));
}
