// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "memutil.hpp"
#include "debug/debug.hpp"

namespace timer
{
	void timer_init();

	enum class Clock
	{
		BusClock,
		BusClockDiv16,
		BusClockDiv256,
		HBlank
	};

	enum class GateType
	{
		HBlank,
		VBlank
	};

	enum class GateMode
	{
		NotActive,
		LowToHigh,
		HighToLow,
		Both
	};

	struct Mode
	{
		Clock clock = Clock::BusClock;
		bool gateEnable = false;
		GateType gateType = GateType::HBlank;
		GateMode gateMode = GateMode::NotActive;
		bool clearOnCompare = false;
		bool timerEnable = false;
		bool interruptOnCompare = false;
		bool interruptOnOverflow = false;
	};

	template <u32 m_timerIndex>
	class Timer
	{
		static constexpr u32 TIMER_BASE = 0xB0000000;
	public:
		static constexpr u32 TIMER_N_COUNT(u32 timerIndex)
		{
			return TIMER_BASE + (timerIndex * 0x800);
		}

		static constexpr u32 TIMER_N_MODE(u32 timerIndex)
		{
			return TIMER_BASE + (timerIndex * 0x800) + 0x10;
		}

		static constexpr u32 TIMER_N_COMPARE(u32 timerIndex)
		{
			return TIMER_BASE + (timerIndex * 0x800) + 0x20;
		}

		static constexpr u32 TIMER_N_HOLD(u32 timerIndex)
		{
			return TIMER_BASE + (timerIndex * 0x800) + 0x30;
		}

		constexpr Timer()
		{
			static_assert(m_timerIndex <= 3, "Invalid timer index");
		}

		constexpr void setCount(u16 count)
		{
			_s16(TIMER_N_COUNT(m_timerIndex), count);
		}

		constexpr u16 getCount() const
		{
			return _l16(TIMER_N_COUNT(m_timerIndex));
		}

		constexpr void setMode(const Mode &mode)
		{
			u16 modeReg = 0;

			switch (mode.clock)
			{
			case Clock::BusClock:
				modeReg |= 0x0;
				break;
			case Clock::BusClockDiv16:
				modeReg |= 0x1;
				break;
			case Clock::BusClockDiv256:
				modeReg |= 0x2;
				break;
			case Clock::HBlank:
				modeReg |= 0x3;
				break;
			}

			if (mode.gateEnable)
			{
				modeReg |= 1 << 2;
			}

			switch (mode.gateType)
			{
			case GateType::HBlank:
				modeReg |= 0 << 3;
				break;
			case GateType::VBlank:
				modeReg |= 1 << 3;
				break;
			}

			switch (mode.gateMode)
			{
			case GateMode::NotActive:
				modeReg |= 0 << 4;
				break;
			case GateMode::LowToHigh:
				modeReg |= 1 << 4;
				break;
			case GateMode::HighToLow:
				modeReg |= 2 << 4;
				break;
			case GateMode::Both:
				modeReg |= 3 << 4;
				break;
			}

			if (mode.clearOnCompare)
			{
				modeReg |= 1 << 6;
			}

			if (mode.timerEnable)
			{
				modeReg |= 1 << 7;
			}

			if (mode.interruptOnCompare)
			{
				modeReg |= 1 << 8;
			}

			if (mode.interruptOnOverflow)
			{
				modeReg |= 1 << 9;
			}

			_s16(TIMER_N_MODE(m_timerIndex), modeReg);
		}

		constexpr void setModeReg(u16 mode)
		{
			_s16(TIMER_N_MODE(m_timerIndex), mode);
		}

		constexpr Mode getMode() const
		{
			u16 modeReg = _l16(TIMER_N_MODE(m_timerIndex));

			Mode mode;

			switch (modeReg & 0x3)
			{
			case 0:
				mode.clock = Clock::BusClock;
				break;
			case 1:
				mode.clock = Clock::BusClockDiv16;
				break;
			case 2:
				mode.clock = Clock::BusClockDiv256;
				break;
			case 3:
				mode.clock = Clock::HBlank;
				break;
			}

			mode.gateEnable = modeReg & (1 << 2);

			switch ((modeReg >> 3) & 1)
			{
			case 0:
				mode.gateType = GateType::HBlank;
				break;
			case 1:
				mode.gateType = GateType::VBlank;
				break;
			}

			switch ((modeReg >> 4) & 3)
			{
			case 0:
				mode.gateMode = GateMode::NotActive;
				break;
			case 1:
				mode.gateMode = GateMode::LowToHigh;
				break;
			case 2:
				mode.gateMode = GateMode::HighToLow;
				break;
			case 3:
				mode.gateMode = GateMode::Both;
				break;
			}

			mode.clearOnCompare = modeReg & (1 << 6);

			if (modeReg & (1 << 7))
			{
				mode.timerEnable = true;
			}

			mode.interruptOnCompare = modeReg & (1 << 8);
			mode.interruptOnOverflow = modeReg & (1 << 9);

			return mode;
		}

		constexpr u16 getModeReg() const
		{
			return _l16(TIMER_N_MODE(m_timerIndex));
		}

		constexpr void setCompare(u16 compare)
		{
			_s16(TIMER_N_COMPARE(m_timerIndex), compare);
		}

		constexpr u16 getCompare() const
		{
			return _l16(TIMER_N_COMPARE(m_timerIndex));
		}

		constexpr void setHold(u16 hold)
		{
			static_assert(m_timerIndex <= 1, "Timer::setHold() - Timer %d does not support hold\n");
			_s16(TIMER_N_HOLD(m_timerIndex), hold);
		}

		constexpr u16 getHold() const
		{
			static_assert(m_timerIndex <= 1, "Timer::getHold() - Timer %d does not support hold\n");
			return _l16(TIMER_N_HOLD(m_timerIndex));
		}

		constexpr void clearInterrupts(bool compare = true, bool overflow = true)
		{
			u16 modeReg = _l16(TIMER_N_MODE(m_timerIndex));

			if (compare)
			{
				modeReg |= (1 << 10);
			}

			if (overflow)
			{
				modeReg |= (1 << 11);
			}

			_s16(TIMER_N_MODE(m_timerIndex), modeReg);
		}

		constexpr bool equalFlag() const
		{
			return _l16(TIMER_N_MODE(m_timerIndex)) & (1 << 10);
		}

		constexpr bool overflowFlag() const
		{
			return _l16(TIMER_N_MODE(m_timerIndex)) & (1 << 11);
		}

		// Helper function, sets bit 7 in MODE
		constexpr void start()
		{
			u16 modeReg = _l16(TIMER_N_MODE(m_timerIndex));
			modeReg |= (1 << 7);
			_s16(TIMER_N_MODE(m_timerIndex), modeReg);
		}

		// Helper function, unsets bit 7 in MODE
		constexpr void stop()
		{
			u16 modeReg = _l16(TIMER_N_MODE(m_timerIndex));
			modeReg &= ~(1 << 7);
			_s16(TIMER_N_MODE(m_timerIndex), modeReg);
		}
	};
}
