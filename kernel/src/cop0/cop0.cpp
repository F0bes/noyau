// SPDX-License-Identifier: MPL-2.0

#include "cop0/cop0.hpp"

namespace cop0
{
	u32 getCop0Reg(u32 reg)
	{
		u32 value;
		asm volatile("mfc0 %0, %1" : "=r"(value) : "r"(reg));
		return value;
	}
} // namespace cop0
