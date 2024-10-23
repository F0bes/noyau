// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "hw_interrupt/common.hpp"

s32 sys_read(s32 fd, u8* buf, size_t count);
s32 sys_write(s32 fd, const u8* buf, size_t count);
s32 sys_open(const char* name, s32 flags);
s32 sys_close(s32 fd);

namespace syscall
{
	typedef void (*sys_call_handler_t)(ee_frame_t*);

	void sys_read(ee_frame_t*);
	void sys_write(ee_frame_t*);
	void sys_open(ee_frame_t*);
	void sys_close(ee_frame_t*);
	void sys_seek(ee_frame_t*);

	#define SYS_HANDLER(num, func) [num] = func

	const sys_call_handler_t sys_call_table[256] =
		{
			SYS_HANDLER(0, sys_read),
			SYS_HANDLER(1, sys_write),
			SYS_HANDLER(2, sys_open),
			SYS_HANDLER(3, sys_close),
			SYS_HANDLER(4, sys_seek)
	};

	#undef SYS_HANDLER
} // namespace syscall
