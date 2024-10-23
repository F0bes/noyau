// SPDX-License-Identifier: MPL-2.0

#include "syscall/syscall.hpp"
#include "fs/vfs.hpp"
#include "debug/debug.hpp"

constexpr u32* ARG(ee_frame_t* frame, size_t n)
{
	switch (n)
	{
		case 0:
			return &frame->a0.w[0];
		case 1:
			return &frame->a1.w[0];
		case 2:
			return &frame->a2.w[0];
		case 3:
			return &frame->a3.w[0];
		case 4:
			return &frame->t0.w[0];
		case 5:
			return &frame->t1.w[0];
		case 6:
			return &frame->t2.w[0];
		case 7:
			return &frame->t3.w[0];
		default: // unreachable
			return nullptr;
	}
}

template <typename T, size_t N>
constexpr T GET_ARG(ee_frame_t* frame)
{
	return *(T*)ARG(frame, N);
}

namespace syscall
{
	void sys_read(ee_frame_t* frame)
	{
		//printi("sys_read\n");
		auto fd = GET_ARG<int, 0>(frame);
		auto buf = GET_ARG<u8*, 1>(frame);
		auto count = GET_ARG<size_t, 2>(frame);
		auto ret = vfs::read(fd, buf, count);

		frame->v0.d[1] = 0;
		frame->v0.d[0] = ret;
	}

	void sys_write(ee_frame_t* frame)
	{
		//printi("sys_write\n");
		auto fd = GET_ARG<int, 0>(frame);
		auto buf = GET_ARG<const u8*, 1>(frame);
		auto count = GET_ARG<size_t, 2>(frame);

		auto ret = vfs::write(fd, buf, count);

		frame->v0.d[1] = 0;
		frame->v0.d[0] = ret;
	}

	void sys_open(ee_frame_t* frame)
	{
		//	printi("sys_open\n");
		auto name = GET_ARG<const char*, 0>(frame);

		s32 fd = vfs::open(name, 0);
		if (fd < 0)
		{
			printe("open failed code %d\n", fd);
		}

		frame->v0.d[1] = 0;
		frame->v0.d[0] = fd;
	}

	void sys_close(ee_frame_t* frame)
	{
		auto fd = GET_ARG<int, 0>(frame);
		s32 ret = vfs::close(fd);
		frame->v0.d[1] = 0;
		frame->v0.d[0] = ret;
	}

	void sys_seek(ee_frame_t* frame)
	{
		auto fd = GET_ARG<int, 0>(frame);
		auto offset = GET_ARG<s64, 1>(frame);
		auto whence = GET_ARG<int, 2>(frame);

		auto ret = vfs::seek(fd, offset, whence);

		frame->v0.d[1] = 0;
		frame->v0.d[0] = ret;
	}
} // namespace syscall

s32 sys_read(s32 fd, u8* buf, size_t count)
{
	s32 ret;
	asm volatile(
		"move $a0, %1\n"
		"move $a1, %2\n"
		"move $a2, %3\n"
		"syscall 0\n"
		"move %0, $v0;\n"
		: "=r"(ret)
		: "r"(fd), "r"(buf), "r"(count)
		: "$v0", "$a0", "$a1", "$a2", "memory");
	return ret;
}

s32 sys_write(s32 fd, const u8* buf, size_t count)
{
	s32 ret;
	asm volatile(
		"move $a0, %1\n"
		"move $a1, %2\n"
		"move $a2, %3\n"
		"syscall 1\n"
		"move %0, $v0;\n"
		: "=r"(ret)
		: "r"(fd), "r"(buf), "r"(count)
		: "$v0", "$a0", "$a1", "$a2", "memory");
	return ret;
}

s32 sys_open(const char* name, s32 flags)
{
	s32 ret;
	asm volatile(
		"move $a0, %1\n"
		"move $a1, %2\n"
		"syscall 2\n"
		"move %0, $v0;\n"
		: "=r"(ret)
		: "r"(name), "r"(flags)
		: "$v0", "$a0", "$a1", "memory");
	return ret;
}

s32 sys_close(s32 fd)
{
	s32 ret;
	asm volatile(
		"move $a0, %1\n"
		"syscall 3\n"
		"move %0, $v0;\n"
		: "=r"(ret)
		: "r"(fd)
		: "$v0", "$a0", "memory");
	return ret;
}

s64 sys_seek(s32 fd, s64 offset, s32 whence)
{
	s32 ret;
	asm volatile(
		"move $a0, %1\n"
		"move $a1, %2\n"
		"move $a2, %3\n"
		"syscall 4\n"
		"move %0, $v0;\n"
		: "=r"(ret)
		: "r"(fd), "r"(offset), "r"(whence)
		: "$v0", "$a0", "$a1", "$a2", "memory");
	return ret;
}
