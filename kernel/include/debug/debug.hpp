// SPDX-License-Identifier: MPL-2.0

#pragma once

#include <source_location>
#include <stdarg.h>
#include "hw_interrupt/common.hpp"

// Quick disabling of source location to rule that out as an issue cause
#define NOYAU_DEBUG_NO_SOURCE_LOCATION
#ifdef NOYAU_DEBUG_NO_SOURCE_LOCATION
#define NOYAU_DEBUG_STR const char*
#else
#define NOYAU_DEBUG_STR sourced_string

struct sourced_string
{
	char str[512];
	std::source_location loc;

	constexpr sourced_string(const char* fmt, std::source_location loc = std::source_location::current())
		: loc(loc)
	{
		for (s32 i = 0; i < 512; i++)
		{
			str[i] = fmt[i];
			if (fmt[i] == '\0')
			{
				break;
			}
		}
	}
};
#endif

class printk
{
	void itoa10(s32 n, char* buf);
	void itoa16(s32 n, char* buf);

public:
	printk(const char* str, ...);
};

// Print Debug
void printd(NOYAU_DEBUG_STR str, ...);
// Print Info
void printi(NOYAU_DEBUG_STR str, ...);
// Print Warning
void printw(NOYAU_DEBUG_STR str, ...);
// Print Error
void printe(NOYAU_DEBUG_STR str, ...);

// Jump to the sio debugger
// The kernel state is as good as dead
void jmpdebug(ee_frame_t* frame);
