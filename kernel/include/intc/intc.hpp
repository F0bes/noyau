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
		SFIFO,
		VU0WD,
		Reserved,
		CAUSE_CNT
	};

	enum DMAC_CAUSE : u8
	{
		DMA_CHANNEL_0,
		DMA_CHANNEL_1,
		DMA_CHANNEL_2,
		DMA_CHANNEL_3,
		DMA_CHANNEL_4,
		DMA_CHANNEL_5,
		DMA_CHANNEL_6,
		DMA_CHANNEL_7,
		DMA_CHANNEL_8,
		DMA_CHANNEL_9,
		DMAC_CAUSE_CNT,
	
		VIF0_DMA = DMA_CHANNEL_0,
		VIF1_DMA = DMA_CHANNEL_1,
		GIF_DMA = DMA_CHANNEL_2,
		IPU_FROM_DMA = DMA_CHANNEL_3,
		IPU_TO_DMA = DMA_CHANNEL_4,
		SIF0_DMA = DMA_CHANNEL_5,
		SIF1_DMA = DMA_CHANNEL_6,
		SIF2_DMA = DMA_CHANNEL_7,
		SPR_FROM_DMA = DMA_CHANNEL_8,
		SPR_TO_DMA = DMA_CHANNEL_9
	};

	constexpr u32 MAX_HANDLERS_PER_INTERRUPT = 8;

	typedef void (*handler_fun_t)(u32 cause);

	// Returns the handler id
	// Returns -1 if the handler could be be registered
	s32 register_handler(CAUSE cause, handler_fun_t handler, bool first = false);
	// Returns the handler id
	// Returns -1 if the handler could be be registered
	s32 register_dmac_handler(DMAC_CAUSE cause, handler_fun_t handler, bool first = false);

	// Returns 0 if successful
	// Returns -1 if the handler could be be unregistered
	s32 unregister_handler(CAUSE cause, s32 handler_id);

	// Returns 0 if successful
	// Returns -1 if the handler could be be unregistered
	s32 unregister_dmac_handler(DMAC_CAUSE cause, s32 handler_id);

	void handle_interrupt(CAUSE cause);
	void handle_dmac_interrupt(DMAC_CAUSE cause);

	void enable_interrupt(CAUSE cause);
	void enable_dmac_interrupt(DMAC_CAUSE cause);
	void disable_interrupt(CAUSE cause);
	void disable_dmac_interrupt(DMAC_CAUSE cause);
} // namespace intc
