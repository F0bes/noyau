// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "mmio/gs.hpp"

#define EARLYFB_WIDTH 640
#define EARLYFB_HEIGHT 448

namespace gs
{
	void gs_init();

	void vterm_init();

	// Hooks the VSYNC interrupt and draws the vterm buffer to the screen
	void vterm_hook();
	void vterm_release();
	void vterm_putc(char c);

} // namespace gs
