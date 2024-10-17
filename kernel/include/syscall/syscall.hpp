// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "hw_interrupt/common.hpp"

int sys_read(int fd, u8* buf, size_t count);
int sys_write(int fd, const u8* buf, size_t count);
int sys_open(const char* name, int flags);
int sys_close(int fd);

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
