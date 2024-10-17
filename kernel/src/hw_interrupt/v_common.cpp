// SPDX-License-Identifier: MPL-2.0

#include "debug/debug.hpp"
#include "memutil.hpp"

#include "syscall/syscall.hpp"

extern "C" {
void _vec_c_common(void);
}

static void v_common_tlb_generic(void)
{
	printd("TLB interrupt from common handler\n");
	jmpdebug(&__excep_interrupt_handler_frame);
}

static void v_common_address_error(void)
{
	printd("Address error interrupt\n");
}

static void v_common_bus_error(void)
{
	printd("Bus error interrupt\n");
}

static void v_common_syscall(void)
{
	u32 op;
	asm volatile(
		"mfc0 $t0, $14\n"
		"lw %0, 0($t0)\n"
		: "=r"(op)::"t0");

	const u32 code = (op >> 6) & 0xfffff;
	syscall::sys_call_table[code](&__excep_interrupt_handler_frame);
	printd("Syscall interrupt code(%d)\n", code);
}

static void v_common_break(void)
{
	printd("Break interrupt\n");
	jmpdebug(&__excep_interrupt_handler_frame);
}

static void v_common_reserved_instruction(void)
{
	printd("Reserved instruction interrupt\n");
}

static void v_common_cop_unusable(void)
{
	printd("COP unusable interrupt\n");
}

static void v_common_overflow(void)
{
	printd("Overflow interrupt\n");
}

static void v_common_trap(void)
{
	printd("Trap interrupt\n");
}

static void v_common_unknown(void)
{
	printd("v_common unknown cause :(\n");
}

static void (*const common_dispatch[])(void) = {
	v_common_unknown,
	v_common_tlb_generic, // tlb modified
	v_common_tlb_generic, // tlb refill (ifetch / load)
	v_common_tlb_generic, // tlb refill (store)
	v_common_address_error, // (ifetch / load)
	v_common_address_error, // (store)
	v_common_bus_error, // (ifetch)
	v_common_bus_error, // (load / store)
	v_common_syscall,
	v_common_break,
	v_common_reserved_instruction,
	v_common_cop_unusable,
	v_common_overflow,
	v_common_trap,
};

constexpr u32 DISPATCH_COUNT = 14;

void _vec_c_common()
{
	u32 ExcCode;
	asm volatile(
		"mfc0 %0, $13\n"
		"srl %0, %0, 2\n"
		"andi %0, %0, 0x1f\n"
		: "=r"(ExcCode));
	if (ExcCode >= DISPATCH_COUNT)
	{
		printk("v_common unknown cause: %d\n", ExcCode);
		v_common_unknown();
		printk("halting\n");
		while (1)
			;
	}
	else
	{
		common_dispatch[ExcCode]();
	}

	// Probably only needed for SYSCALL,BREAK,TRAP (overflow?) cases
	asm volatile(
		"mfc0 $t0, $14\n"
		"addi $t0, $t0, 4\n"
		"mtc0 $t0, $14\n");
}
