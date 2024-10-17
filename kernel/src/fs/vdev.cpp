// SPDX-License-Identifier: MPL-2.0

#include "debug/debug.hpp"
#include "fs/vdev.hpp"
#include "util/util.hpp"

namespace vdev
{
	vnode_t* vdevs[VDEV_MAX_DEVICES];

	vnode_t* vdev_find(const char* name)
	{
		for (size_t i = 0; i < VDEV_MAX_DEVICES; i++)
		{
			if (vdevs[i] == nullptr)
			{
				continue;
			}
			if (kstrcmp(vdevs[i]->name, name) == 0)
			{
				return vdevs[i];
			}
		}
		return nullptr;
	}

	bool vdev_register(vnode_t* vnode)
	{
		if (vdev_find(vnode->name) != nullptr)
		{
			return false;
		}

		for (size_t i = 0; i < VDEV_MAX_DEVICES; i++)
		{
			if (vdevs[i] == nullptr)
			{
				vdevs[i] = vnode;
				return true;
			}
		}

		return false;
	}

	bool vdev_unregister(const char* name)
	{
		for (size_t i = 0; i < VDEV_MAX_DEVICES; i++)
		{
			if (kstrcmp(vdevs[i]->name, name) == 0)
			{
				vdevs[i] = nullptr;
				return true;
			}
		}
		return false;
	}

	vnode_t stdout_vnode = {
		.name = "/dev/stdout",
		.open = [](int flags) -> int {
			return 0;
		},
		.close = [](int fd) -> int {
			return 0;
		},
		.read = [](int fd, void* buf, size_t count) -> int {
			return 0;
		},
		.write = [](int fd, const void* buf, size_t count) -> int {
			printk("%s", (const char*)buf);
			return count;
		},
		.seek = nullptr};

	vnode_t null_vnode = {
		.name = "/dev/null",
		.open = [](int flags) -> int {
			return 0;
		},
		.close = [](int fd) -> int {
			return 0;
		},
		.read = [](int fd, void* buf, size_t count) -> int {
			return 0;
		},
		.write = [](int fd, const void* buf, size_t count) -> int {
			return count;
		},
		.seek = nullptr};

	vnode_t random_vnode = {
		.name = "/dev/random",
		.open = [](int flags) -> int {
			return 0;
		},
		.close = [](int fd) -> int {
			return 0;
		},
		.read = [](int fd, void* buf, size_t count) -> int {
			// Just use processor cycle counter as a source of randomness
			u32 cop0_count;
			asm volatile("mfc0 %0, $9" : "=r"(cop0_count));

			for (size_t i = 0; i < count; i++)
			{
				((u8*)buf)[i] = cop0_count & 0xff;
				cop0_count >>= 8;
				if (i % 4 == 0)
				{
					asm volatile("mfc0 %0, $9" : "=r"(cop0_count));
				}
			}
			return count;
		},
		.write = [](int fd, const void* buf, size_t count) -> int {
			return count;
		},
		.seek = nullptr};

	vnode_t zero_vnode = {
		.name = "/dev/zero",
		.open = [](int flags) -> int {
			return 0;
		},
		.close = [](int fd) -> int {
			return 0;
		},
		.read = [](int fd, void* buf, size_t count) -> int {
			for (size_t i = 0; i < count; i++)
			{
				((u8*)buf)[i] = 0;
			}
			return count;
		},
		.write = [](int fd, const void* buf, size_t count) -> int {
			return count;
		},
		.seek = nullptr};

	void vdev_init()
	{
		for (size_t i = 0; i < VDEV_MAX_DEVICES; i++)
		{
			vdevs[i] = nullptr;
		}

		vdev_register(&stdout_vnode);
		vdev_register(&null_vnode);
		vdev_register(&random_vnode);
		vdev_register(&zero_vnode);

		printd("vdev_init done\n");
	}
}; // namespace vdev
