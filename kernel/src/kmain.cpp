// SPDX-License-Identifier: MPL-2.0

#include "memutil.hpp"
#include "debug/debug.hpp"
#include "timer/timer.hpp"
#include "syscall/syscall.hpp"
#include "fs/vfs.hpp"

#include "util/printf.h"

#include "gs/gs.hpp"

void kmain_real(void)
{
	vfs::vfs_init();
	timer::timer_init();
	gs::gs_init();
	printi("Noyau inited\n");


	int fb = vfs::open("/dev/fb", 0);
	int random = vfs::open("/dev/random", 0);
	u32 line[640];
	for (size_t i = 0; i < 448; i++)
	{
		vfs::read(random, line, sizeof(line));
		vfs::write(fb, line, sizeof(line));
	}
	vfs::close(fb);

	asm volatile("break");
	while (1)
		;
}

extern "C" {
void kmain_bootstrap(void)
{
	kmain_real();
}
}
