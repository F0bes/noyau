// SPDX-License-Identifier: MPL-2.0

#pragma once

namespace sif
{
	enum STATUS
	{
		SIF_STATUS_SIF_INIT = 0x10000,
		SIF_STATUS_CMD_INIT = 0x20000,
		SIF_STATUS_BOOT_END = 0x40000,
	};

	enum COMMAND
	{
		SIF_CMD_SYS = 0x80000000,
		SIF_CMD_SYS_CHANGE_SADDR = SIF_CMD_SYS,
		SIF_CMD_SYS_SREG,
		SIF_CMD_SYS_INIT,
		SIF_CMD_SYS_RESET
	};

	constexpr u32 SYS_ID_TO_ID(u32 id) { return id & ~SIF_CMD_SYS; }

	void sif_init();
} // namespace sif
