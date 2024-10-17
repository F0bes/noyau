// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "vdev.hpp"

namespace vfs
{
	constexpr int MAX_FD = 128;

	struct file_descriptor_t
	{
		vdev::vnode_t* vnode;
		int flags;
		int refs;
		s64 pos;
	};

	enum
	{
		SEEK_SET,
		SEEK_CUR,
		SEEK_END
	};

	void vfs_init();

	int fd_alloc(vdev::vnode_t* vnode, int flags);
	void fd_free(int fd);
	file_descriptor_t& get_fd(int fd);

	int open(const char* name, int flags);
	int close(int fd);
	size_t read(int fd, void* buf, size_t count);
	size_t write(int fd, const void* buf, size_t count);
	s64 seek(int fd, s64 offset, int whence);
} // namespace vfs
