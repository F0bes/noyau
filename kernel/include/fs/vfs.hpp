// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "vdev.hpp"

namespace vfs
{
	constexpr s32 MAX_FD = 128;

	struct file_descriptor_t
	{
		vdev::vnode_t* vnode;
		s32 flags;
		s32 refs;
		s64 pos;
	};

	enum
	{
		SEEK_SET,
		SEEK_CUR,
		SEEK_END
	};

	void vfs_init();

	s32 fd_alloc(vdev::vnode_t* vnode, s32 flags);
	void fd_free(s32 fd);
	file_descriptor_t& get_fd(s32 fd);

	s32 open(const char* name, s32 flags);
	s32 close(s32 fd);
	size_t read(s32 fd, void* buf, size_t count);
	size_t write(s32 fd, const void* buf, size_t count);
	s64 seek(s32 fd, s64 offset, s32 whence);
} // namespace vfs
