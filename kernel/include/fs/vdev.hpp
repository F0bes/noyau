// SPDX-License-Identifier: MPL-2.0

#pragma once

#include <cstddef>

namespace vdev
{
	// /dev/null, /dev/random, etc
	constexpr size_t VDEV_MAX_DEVICES = 32;

	struct vnode_t
	{
		const char* name;
		int (*open)(int flags);
		int (*close)(int fd);
		int (*read)(int fd, void* buf, size_t count);
		int (*write)(int fd, const void* buf, size_t count);
		int (*seek)(int fd, s64 offset, int whence);
	};

	void vdev_init();
	vnode_t* vdev_find(const char* name);
	bool vdev_register(vnode_t* vnode);
	bool vdev_unregister(const char* name);
}; // namespace vdev
