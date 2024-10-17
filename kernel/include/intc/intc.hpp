// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "debug/debug.hpp"

namespace intc
{
	enum CAUSE : u8
	{
		GS,
		SBUS,
		VBON,
		VBOF,
		VIF0,
		VIF1,
		VU0,
		VU1,
		IPU,
		TIM0,
		TIM1,
		TIM2,
		TIM3,
		SIF0,
		VU0WD,
		Reserved,
		CAUSE_CNT
	};

	constexpr u32 MAX_HANDLERS_PER_INTERRUPT = 8;

	typedef void (*handler_fun_t)(u32 cause);

	// Returns the handler id
	// Returns -1 if the handler could be be registered
	s32 register_handler(CAUSE cause, handler_fun_t handler, bool first = false);
	// Returns 0 if successful
	// Returns -1 if the handler could be be unregistered
	s32 unregister_handler(CAUSE cause, s32 handler_id);
	void handle_interrupt(CAUSE cause);

	void enable_interrupt(CAUSE cause);
	void disable_interrupt(CAUSE cause);
} // namespace intc
