// SPDX-License-Identifier: MPL-2.0

#include "memutil.hpp"
#include "debug/debug.hpp"
#include "timer/timer.hpp"
#include "syscall/syscall.hpp"
#include "fs/vfs.hpp"
#include "sif/sif.hpp"

#include "gs/gs.hpp"

extern "C" {
void kmain_bootstrap(void)
{
	vfs::vfs_init();
	timer::timer_init();
	gs::gs_init();
	gs::vterm_init();
	gs::vterm_hook();
	sif::sif_init();
	asm volatile("ei");
	printi("Noyau inited\n");

	while (1)
		;
	asm volatile("break");
	while (1)
		;
}
}
