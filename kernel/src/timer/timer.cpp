// SPDX-License-Identifier: MPL-2.0

#include "timer/timer.hpp"
#include "debug/debug.hpp"

#include "fs/vdev.hpp"
#include "fs/vfs.hpp"

#include "util/util.hpp"

#include <array>

namespace timer
{
	void init_timer_vdevs();

	constexpr u8 TIMER_COUNT = 4;

	template <s32 n>
	constexpr void init_timer_n()
	{
		Timer<n> timer;
		timer.setCount(0);
		timer.setMode(Mode());
		timer.setCompare(0);
		if constexpr (n <= 1)
		{
			timer.setHold(0);
		}
	}

	template <s32 n>
	constexpr void init_n_timers()
	{
		if constexpr (n >= 0)
		{
			init_timer_n<n>();
			init_n_timers<n - 1>();
		}
	}

	void timer_init()
	{
		printk("timer_init() - Initializing timers\n");
		init_n_timers<TIMER_COUNT - 1>();
		init_timer_vdevs();
		printk("timer_init() - Initialized %d timers\n", TIMER_COUNT);
	}

#define GEN_TIMER_VDEV(n) \
	vdev::vnode_t tim_count_##n = { \
		.name = "/dev/tim" #n "/count", \
		.open = nullptr, \
		.close = nullptr, \
		.read = [](s32 fd, void* buf, size_t count) -> s32 { \
			if (count != sizeof(u16)) \
			{ \
				return -1; \
			} \
			auto timer = Timer<n>(); \
			u16 val = timer.getCount(); \
			*(u16*)buf = val; \
			return sizeof(val); \
		}, \
		.write = [](s32 fd, const void* buf, size_t count) -> s32 { \
			if (count != sizeof(u16)) \
			{ \
				return -1; \
			} \
			auto timer = Timer<n>(); \
			u16 val = *(u16*)buf; \
			timer.setCount(val); \
			return sizeof(val); \
		}, \
		.seek = nullptr}; \
\
	vdev::vnode_t tim_compare_##n = { \
		.name = "/dev/tim" #n "/compare", \
		.open = nullptr, \
		.close = nullptr, \
		.read = [](s32 fd, void* buf, size_t count) -> s32 { \
			auto timer = Timer<n>(); \
			u16 val = timer.getCompare(); \
			*(u16*)buf = val; \
			return sizeof(u16); \
		}, \
		.write = [](s32 fd, const void* buf, size_t count) -> s32 { \
			auto timer = Timer<n>(); \
			auto val = *(u32*)buf; \
			timer.setCompare(val); \
			return sizeof(val); \
		}, \
		.seek = nullptr}; \
\
	vdev::vnode_t tim_mode_##n = { \
		.name = "/dev/tim" #n "/mode", \
		.open = nullptr, \
		.close = nullptr, \
		.read = [](s32 fd, void* buf, size_t count) -> s32 { \
			if (count != sizeof(u16)) \
			{ \
				return -1; \
			} \
			auto timer = Timer<n>(); \
			u16 val = timer.getModeReg(); \
			*(u16*)buf = val; \
			return sizeof(u16); \
		}, \
		.write = [](s32 fd, const void* buf, size_t count) -> s32 { \
			if (count != sizeof(u16)) \
			{ \
				return -1; \
			} \
			auto timer = Timer<n>(); \
			u16 val = *(u32*)buf; \
			timer.setModeReg(val); \
			return sizeof(u16); \
		}, \
		.seek = nullptr};

#define GEN_TIMER_VDEV_HOLD(n) \
	vdev::vnode_t tim_hold_##n = { \
		.name = "/dev/tim" #n "/hold", \
		.open = nullptr, \
		.close = nullptr, \
		.read = [](s32 fd, void* buf, size_t count) -> s32 { \
			if (count != sizeof(u16)) \
			{ \
				return -1; \
			} \
			auto timer = Timer<n>(); \
			u16 val = timer.getHold(); \
			*(u16*)buf = val; \
			return sizeof(u16); \
		}, \
		.write = [](s32 fd, const void* buf, size_t count) -> s32 { \
			if (count != sizeof(u16)) \
			{ \
				return -1; \
			} \
			auto timer = Timer<n>(); \
			u16 val = *(u16*)buf; \
			timer.setHold(val); \
			return sizeof(val); \
		}, \
		.seek = nullptr};

	GEN_TIMER_VDEV(0)
	GEN_TIMER_VDEV(1)
	GEN_TIMER_VDEV(2)
	GEN_TIMER_VDEV(3)
	GEN_TIMER_VDEV_HOLD(0)
	GEN_TIMER_VDEV_HOLD(1)
#undef GEN_TIMER_VDEV
#undef GEN_TIMER_VDEV_HOLD

	void init_timer_vdevs()
	{
		printi("Registering hardware timer vdevs\n");
		vdev::vdev_register(&tim_count_0);
		vdev::vdev_register(&tim_compare_0);
		vdev::vdev_register(&tim_mode_0);
		vdev::vdev_register(&tim_hold_0);

		vdev::vdev_register(&tim_count_1);
		vdev::vdev_register(&tim_compare_1);
		vdev::vdev_register(&tim_mode_1);
		vdev::vdev_register(&tim_hold_1);

		vdev::vdev_register(&tim_count_2);
		vdev::vdev_register(&tim_compare_2);
		vdev::vdev_register(&tim_mode_2);

		vdev::vdev_register(&tim_count_3);
		vdev::vdev_register(&tim_compare_3);
		vdev::vdev_register(&tim_mode_3);

		printi("Registered hardware timer vdevs\n");
	}
} // namespace timer
