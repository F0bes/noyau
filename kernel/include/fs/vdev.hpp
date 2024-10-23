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
		s32 (*open)(s32 flags);
		s32 (*close)(s32 fd);
		s32 (*read)(s32 fd, void* buf, size_t count);
		s32 (*write)(s32 fd, const void* buf, size_t count);
		s32 (*seek)(s32 fd, s64 offset, s32 whence);
	};

	void vdev_init();
	vnode_t* vdev_find(const char* name);
	bool vdev_register(vnode_t* vnode);
	bool vdev_unregister(const char* name);
}; // namespace vdev
