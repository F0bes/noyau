// SPDX-License-Identifier: MPL-2.0

#include "memutil.hpp"
#include "debug/debug.hpp"
#include "util/util.hpp"
#include "util/printf.h"
#include "sio/sio.hpp"

#define DBG_VA_INIT() \
	va_list args; \
	va_start(args, str);

#define DBG_VA_END() \
	va_end(args);

printk::printk(const char* str, ...)
{
	DBG_VA_INIT()
	vprintf(str, args);
	DBG_VA_END()
}

void printd(NOYAU_DEBUG_STR str, ...)
{
	DBG_VA_INIT()

#ifndef NOYAU_DEBUG_NO_SOURCE_LOCATION
	printk("[D] %s: ", str.loc.function_name());
	vprintf(str.str, args);
#else
	printk("[D] ");
	vprintf(str, args);
#endif

	DBG_VA_END()
}

void printi(NOYAU_DEBUG_STR str, ...)
{
	DBG_VA_INIT()

#ifndef NOYAU_DEBUG_NO_SOURCE_LOCATION
	printk("[I] %s: ", str.loc.function_name());
	vprintf(str.str, args);
#else
	printk("[I] ");
	vprintf(str, args);
#endif

	DBG_VA_END()
}

void printw(NOYAU_DEBUG_STR str, ...)
{
	DBG_VA_INIT()

#ifndef NOYAU_DEBUG_NO_SOURCE_LOCATION
	printk("[W] %s: ", str.loc.function_name());
	vprintf(str.str, args);
#else
	printk("[W] ");
	vprintf(str, args);
#endif

	DBG_VA_END()
}

void printe(NOYAU_DEBUG_STR str, ...)
{
	DBG_VA_INIT()

#ifndef NOYAU_DEBUG_NO_SOURCE_LOCATION
	printk("[E] %s: ", str.loc.function_name());
	vprintf(str.str, args);
#else
	printk("[E] ");
	vprintf(str, args);
#endif

	DBG_VA_END()
}

static void dump_regs(ee_frame_t* frame)
{
	printk("# DUMPING FRAME\n");

	printk("at: 0x%08x %08x %08x %08x\t v0: 0x%08x %08x %08x %08x\n",
		frame->at.w[3], frame->at.w[2], frame->at.w[1], frame->at.w[0],
		frame->v0.w[3], frame->v0.w[2], frame->v0.w[1], frame->v0.w[0]);
	printk("v1: 0x%08x %08x %08x %08x\t a0: 0x%08x %08x %08x %08x\n",
		frame->v1.w[3], frame->v1.w[2], frame->v1.w[1], frame->v1.w[0],
		frame->a0.w[3], frame->a0.w[2], frame->a0.w[1], frame->a0.w[0]);

	printk("a1: 0x%08x %08x %08x %08x\t a2: 0x%08x %08x %08x %08x\n",
		frame->a1.w[3], frame->a1.w[2], frame->a1.w[1], frame->a1.w[0],
		frame->a2.w[3], frame->a2.w[2], frame->a2.w[1], frame->a2.w[0]);

	printk("a3: 0x%08x %08x %08x %08x\t a4: 0x%08x %08x %08x %08x\n",
		frame->a3.w[3], frame->a3.w[2], frame->a3.w[1], frame->a3.w[0],
		frame->a4.w[3], frame->a4.w[2], frame->a4.w[1], frame->a4.w[0]);

	printk("a5: 0x%08x %08x %08x %08x\t a6: 0x%08x %08x %08x %08x\n",
		frame->a5.w[3], frame->a5.w[2], frame->a5.w[1], frame->a5.w[0],
		frame->a6.w[3], frame->a6.w[2], frame->a6.w[1], frame->a6.w[0]);

	printk("a7: 0x%08x %08x %08x %08x\t t0: 0x%08x %08x %08x %08x\n",
		frame->a7.w[3], frame->a7.w[2], frame->a7.w[1], frame->a7.w[0],
		frame->t0.w[3], frame->t0.w[2], frame->t0.w[1], frame->t0.w[0]);

	printk("t1: 0x%08x %08x %08x %08x\t t2: 0x%08x %08x %08x %08x\n",
		frame->t1.w[3], frame->t1.w[2], frame->t1.w[1], frame->t1.w[0],
		frame->t2.w[3], frame->t2.w[2], frame->t2.w[1], frame->t2.w[0]);

	printk("t3: 0x%08x %08x %08x %08x\t s0: 0x%08x %08x %08x %08x\n",
		frame->t3.w[3], frame->t3.w[2], frame->t3.w[1], frame->t3.w[0],
		frame->s0.w[3], frame->s0.w[2], frame->s0.w[1], frame->s0.w[0]);

	printk("s1: 0x%08x %08x %08x %08x\t s2: 0x%08x %08x %08x %08x\n",
		frame->s1.w[3], frame->s1.w[2], frame->s1.w[1], frame->s1.w[0],
		frame->s2.w[3], frame->s2.w[2], frame->s2.w[1], frame->s2.w[0]);

	printk("s3: 0x%08x %08x %08x %08x\t s4: 0x%08x %08x %08x %08x\n",
		frame->s3.w[3], frame->s3.w[2], frame->s3.w[1], frame->s3.w[0],
		frame->s4.w[3], frame->s4.w[2], frame->s4.w[1], frame->s4.w[0]);

	printk("s5: 0x%08x %08x %08x %08x\t s6: 0x%08x %08x %08x %08x\n",
		frame->s5.w[3], frame->s5.w[2], frame->s5.w[1], frame->s5.w[0],
		frame->s6.w[3], frame->s6.w[2], frame->s6.w[1], frame->s6.w[0]);

	printk("s7: 0x%08x %08x %08x %08x\t t8: 0x%08x %08x %08x %08x\n",
		frame->s7.w[3], frame->s7.w[2], frame->s7.w[1], frame->s7.w[0],
		frame->t8.w[3], frame->t8.w[2], frame->t8.w[1], frame->t8.w[0]);

	printk("t9: 0x%08x %08x %08x %08x\t k0: 0x%08x %08x %08x %08x\n",
		frame->t9.w[3], frame->t9.w[2], frame->t9.w[1], frame->t9.w[0],
		frame->k0.w[3], frame->k0.w[2], frame->k0.w[1], frame->k0.w[0]);

	printk("k1: 0x%08x %08x %08x %08x\t gp: 0x%08x %08x %08x %08x\n",
		frame->k1.w[3], frame->k1.w[2], frame->k1.w[1], frame->k1.w[0],
		frame->gp.w[3], frame->gp.w[2], frame->gp.w[1], frame->gp.w[0]);

	printk("sp: 0x%08x %08x %08x %08x\t fp: 0x%08x %08x %08x %08x\n",
		frame->sp.w[3], frame->sp.w[2], frame->sp.w[1], frame->sp.w[0],
		frame->fp.w[3], frame->fp.w[2], frame->fp.w[1], frame->fp.w[0]);

	u32 epc = 0;
	asm volatile("mfc0 %0, $14" : "=r"(epc));
	printk("ra: 0x%08x %08x %08x %08x\t epc: 0x%08x\n",
		frame->ra.w[3], frame->ra.w[2], frame->ra.w[1], frame->ra.w[0],
		epc);
}

void jmpdebug(ee_frame_t* frame)
{
	// Clear any junk in the RX FIFO
	while (sio::sio_getc(false) != -1)
		;
	printk("##### NOYAU DEBUGGER #####\n");

	dump_regs(frame);

	char in_buf[256];
	while (1)
	{
		sio::sio_getsn((char*)in_buf, 256);
		printk("dbg> %s\n", in_buf);

		if (kstrstr(in_buf, "l32") == in_buf)
		{
			char* addr_str = in_buf + 4;
			u64 addr = strtol(addr_str, &addr_str, 16);
			printk("0x%08lx = 0x%08lx\n", addr, _l32(addr));
		}
		else if (kstrstr(in_buf, "s32") == in_buf)
		{
			char* addr_str = in_buf + 4;
			u64 addr = strtol(addr_str, &addr_str, 16);
			char* val_str = addr_str + 1;
			u64 val = strtol(val_str, &val_str, 16);
			printk("0x%08lx -> 0x%08lx\n", addr, val);
			_s32(addr, val);
		}
		else if (kstrstr(in_buf, "l16") == in_buf)
		{
			char* addr_str = in_buf + 4;
			u64 addr = strtol(addr_str, &addr_str, 16);
			printk("0x%08lx = 0x%04lx\n", addr, _l16(addr));
		}
		else if (kstrstr(in_buf, "s16") == in_buf)
		{
			char* addr_str = in_buf + 4;
			u64 addr = strtol(addr_str, &addr_str, 16);
			char* val_str = addr_str + 1;
			u64 val = strtol(val_str, &val_str, 16);
			printk("0x%08lx -> 0x%04lx\n", addr, val);
			_s16(addr, val);
		}
		else if (kstrstr(in_buf, "l8") == in_buf)
		{
			char* addr_str = in_buf + 3;
			u64 addr = strtol(addr_str, &addr_str, 16);
			printk("0x%08lx = 0x%02lx\n", addr, _l8(addr));
		}
		else if (kstrstr(in_buf, "s8") == in_buf)
		{
			char* addr_str = in_buf + 3;
			u64 addr = strtol(addr_str, &addr_str, 16);
			char* val_str = addr_str + 1;
			u64 val = strtol(val_str, &val_str, 16);
			printk("0x%08lx -> 0x%02lx\n", addr, val);
			_s8(addr, val);
		}
		else if (kstrstr(in_buf, "help") == in_buf)
		{
			printk("l32 <addr> - Load 32-bit value from address\n");
			printk("s32 <addr> <val> - Store 32-bit value to address\n");
			printk("l16 <addr> - Load 16-bit value from address\n");
			printk("s16 <addr> <val> - Store 16-bit value to address\n");
			printk("l8 <addr> - Load 8-bit value from address\n");
			printk("s8 <addr> <val> - Store 8-bit value to address\n");
		}
		else if (kstrstr(in_buf, "exit") == in_buf)
		{
			break;
		}
		else
		{
			printk("Unknown command %s\n", in_buf);
		}
	}
}
