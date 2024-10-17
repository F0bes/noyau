// SPDX-License-Identifier: MPL-2.0

#include "intc/intc.hpp"
#include "mmio/intc.hpp"
#include "debug/debug.hpp"
#include "timer/timer.hpp"

extern "C" {
void _vec_c_interrupt(void);
}

static void v_int_reserved(void)
{
	printd("Reserved interrupt\n");
}

static void v_int_vu0wd(void)
{
	printd("VU0 Watchdog interrupt\n");
}

static void v_int_sif0(void)
{
	printd("SIF0 interrupt\n");
}

static void v_int_tim3(void)
{
	timer::Timer<3>().clearInterrupts();
	_s32(I_STAT, 1 << intc::TIM3);
	printd("Timer 3 interrupt\n");
}

static void v_int_tim2(void)
{
	timer::Timer<2>().clearInterrupts();
	_s32(I_STAT, 1 << intc::TIM2);
	printd("Timer 2 interrupt\n");
}

static void v_int_tim1(void)
{
	timer::Timer<1>().clearInterrupts();
	_s32(I_STAT, 1 << intc::TIM1);
	printd("Timer 1 interrupt\n");
}

static void v_int_tim0(void)
{
	timer::Timer<0>().clearInterrupts();
	_s32(I_STAT, 1 << intc::TIM0);
	printd("Timer 0 interrupt\n");
}

static void v_int_ipu(void)
{
	printd("IPU interrupt\n");
}

static void v_int_vu1(void)
{
	printd("VU1 interrupt\n");
}

static void v_int_vu0(void)
{
	printd("VU0 interrupt\n");
}

static void v_int_vif1(void)
{
	printd("VIF1 interrupt\n");
}

static void v_int_vif0(void)
{
	printd("VIF0 interrupt\n");
}

static void v_int_vbof(void)
{
	/*
		Not really sure, but it seems that the VBOF and VBON interrupts
		completely ignore the I_MASK register...
	*/
	u32 i_mask = _l32(0x1000f010);
	if (i_mask & (1 << 3))
	{
		printd("VBOF interrupt\n");
	}
}

static void v_int_vbon(void)
{
	/*
		Not really sure, but it seems that the VBOF and VBON interrupts
		completely ignore the I_MASK register...
	*/
	u32 i_mask = _l32(0x1000f010);
	if (i_mask & (1 << 2))
	{
		printd("VBOF interrupt\n");
	}
}

static void v_int_sbus(void)
{
	printd("SBUS interrupt\n");
}

static void v_int_gs(void)
{
	printd("GS interrupt\n");
}

static void (*const intc_dispatch[])(void) = {
	v_int_reserved, // Reserved
	v_int_vu0wd, // VU0WD
	v_int_sif0, // SIF0
	v_int_tim3, // TIM3
	v_int_tim2, // TIM2
	v_int_tim1, // TIM1
	v_int_tim0, // TIM0
	v_int_ipu, // IPU
	v_int_vu1, // VU1
	v_int_vu0, // VU0
	v_int_vif1, // VIF1
	v_int_vif0, // VIF0
	v_int_vbof, // VBOF
	v_int_vbon, // VBON
	v_int_sbus, // SBUS
	v_int_gs // GS
};

static inline u32 _plzcw(u32 val)
{
	u32 ret;
	asm volatile("plzcw %0, %1" : "=r"(ret) : "r"(val));
	return ret;
}

void _vec_c_interrupt(void)
{
	u32 cause;
	asm volatile("mfc0 %0, $13" : "=r"(cause));

	// INT0 (INTC)
	if ((cause >> 10) & 1)
	{
		const u32 i_stat = _l32(0x1000f000);
		const u32 lz = _plzcw(i_stat) - 15;
		if (lz > 15)
		{
			printe("v_interrupt INT0 out of range. I_STAT = %x\n", i_stat);
		}
		else
		{
			intc_dispatch[lz]();
			_s32(0x1000f000, 1 << (15 - lz));
		}
	}

	// INT1 (DMAC)
	else if ((cause >> 11) & 1)
	{
		printe("v_interrupt INT1 not implemented\n");
	}

	// COP0 timer interrupt
	else if ((cause >> 15) & 1)
	{
		printe("v_interrupt COP0 timer interrupt not implemented\n");
		// "As a side effect, writing to the Compare register clears the timer interrupt."
		asm volatile("mtc0 $zero, $11");
	}
}
