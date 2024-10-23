// SPDX-License-Identifier: MPL-2.0

#include "fs/vfs.hpp"
#include "fs/vdev.hpp"
#include "debug/debug.hpp"

using namespace vdev;

namespace vfs
{
	static file_descriptor_t fd_table[MAX_FD];

	void vfs_init()
	{
		for (s32 i = 0; i < MAX_FD; i++)
		{
			fd_table[i].vnode = nullptr;
			fd_table[i].flags = 0;
			fd_table[i].refs = 0;
			fd_table[i].pos = 0;
		}
		vdev::vdev_init();
	}

	s32 fd_alloc(vnode_t* vnode, s32 flags)
	{
		for (s32 i = 3; i < MAX_FD; i++)
		{
			if (fd_table[i].vnode == nullptr)
			{
				fd_table[i].vnode = vnode;
				fd_table[i].flags = flags;
				fd_table[i].pos = 0;
				fd_table[i].refs = 1;
				return i;
			}
		}
		return -1;
	}

	void fd_free(s32 fd)
	{
		if (fd_table[fd].refs == 1)
		{
			fd_table[fd].vnode = nullptr;
			fd_table[fd].flags = 0;
			fd_table[fd].refs = 0;
		}
		else
		{
			fd_table[fd].refs--;
		}
	}

	file_descriptor_t& get_fd(s32 fd)
	{
		return fd_table[fd];
	}

	s32 open(const char* name, s32 flags)
	{
		vnode_t* vnode = vdev::vdev_find(name);
		if (vnode == nullptr)
		{
			printe("vnode %s not found\n", name);
			return -1;
		}

		const s32 fd = fd_alloc(vnode, flags);
		if (fd < 0)
		{
			printe("fd allocation failed\n");
			return fd;
		}

		if (vnode->open != nullptr)
		{
			const s32 v_open_ret = vnode->open(flags);
			if (v_open_ret < 0)
			{
				printe("vnode open failed\n");
				fd_free(fd);
				return v_open_ret;
			}
		}

		printd("fd %d opened for %s\n", fd, name);
		return fd;
	}

	s32 close(s32 fd)
	{
		if (fd < 0 || fd >= MAX_FD)
		{
			printe("fd %d is out of range\n", fd);
			return -1;
		}

		if (fd_table[fd].vnode == nullptr)
		{
			printe("fd %d is not allocated\n", fd);
			return -1;
		}

		if (fd_table[fd].vnode->close != nullptr)
		{
			const s32 v_close_ret = fd_table[fd].vnode->close(fd);
			if (v_close_ret < 0)
			{
				printe("vnode close failed\n");
				return v_close_ret;
			}
		}
		printd("fd %d closed\n", fd);
		fd_free(fd);
		return 0;
	}

	size_t read(s32 fd, void* buf, size_t count)
	{
		if (fd < 0 || fd >= MAX_FD)
		{
			printe("fd %d is out of range\n", fd);
			return -1;
		}

		if (fd_table[fd].vnode == nullptr)
		{
			printe("fd %d is not allocated\n", fd);
			return -1;
		}

		if (fd_table[fd].vnode->read != nullptr)
		{
			const s32 v_read_ret = fd_table[fd].vnode->read(fd, buf, count);
			if (v_read_ret < 0)
			{
				printe("vnode read failed\n");
				return v_read_ret;
			}
			printd("fd %d read %d bytes\n", fd, v_read_ret);
			return v_read_ret;
		}
		else
		{
			printd("fd %d has no read function\n", fd);
		}
		return 0;
	}

	size_t write(s32 fd, const void* buf, size_t count)
	{
		if (fd < 0 || fd >= MAX_FD)
		{
			printe("fd %d is out of range\n", fd);
			return -1;
		}

		if (fd_table[fd].vnode == nullptr)
		{
			printe("fd %d is not allocated\n", fd);
			return -1;
		}

		if (fd_table[fd].vnode->write != nullptr)
		{
			const s32 v_write_ret = fd_table[fd].vnode->write(fd, buf, count);
			if (v_write_ret < 0)
			{
				printe("vnode write failed\n");
				return v_write_ret;
			}
			printd("fd %d wrote %d bytes\n", fd, v_write_ret);
			return v_write_ret;
		}
		else
		{
			printd("fd %d has no write function\n", fd);
		}
		return 0;
	}

	s64 seek(s32 fd, s64 offset, s32 whence)
	{
		if (fd < 0 || fd >= MAX_FD)
		{
			printe("fd %d is out of range\n", fd);
			return -1;
		}

		if (fd_table[fd].vnode == nullptr)
		{
			printe("fd %d is not allocated\n", fd);
			return -1;
		}

		if (fd_table[fd].vnode->seek != nullptr)
		{
			const s32 v_seek_ret = fd_table[fd].vnode->seek(fd, offset, whence);
			if (v_seek_ret < 0)
			{
				printe("vnode seek failed\n");
				return v_seek_ret;
			}
			return v_seek_ret;
		}
		else
		{
			printd("fd %d has no seek function\n", fd);
		}
		return 0;
	}
} // namespace vfs
