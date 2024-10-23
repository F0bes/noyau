// SPDX-License-Identifier: MPL-2.0

#include "elf32.h"

#include <kernel.h>
#include <sio.h>
#include <string.h>
#include <stdio.h>

extern Elf32_Size size_kelf_data;
extern char kelf_data[];

void writeback_dcache()
{
	for (s32 i = 0; i < 128; i++)
	{
		__asm__ volatile("sync\n"
						 "cache 0x14, 0(%0)\n"
						 "sync\n"
						 "cache 0x14, 1(%0)\n"
						 "sync\n" ::"r"(i * 64));
	}
}

void invalidate_icache()
{
	for (s32 i = 0; i < 256; i++)
	{
		__asm__ volatile("sync\n"
						 "cache 0x7, 0(%0)\n"
						 "sync\n"
						 "cache 0x7, 1(%0)\n"
						 "sync\n"
						 "cache 0xC, 0($zero)" ::"r"(i * 64));
	}
}

s32 main(void)
{
	sio_puts("==== Sploot Loader ====");
	sio_puts("Loading kernel...");

	Elf32_Ehdr* ehdr = (Elf32_Ehdr*)kelf_data;
	uint32_t ep = ehdr->e_entry;

	if (!IS_ELF(*ehdr))
	{
		while (1)
			;
	}

	DI();
	ee_kmode_enter();

	Elf32_Phdr* phdr = (Elf32_Phdr*)((uintptr_t)ehdr + ehdr->e_phoff);

	for (s32 i = 0; i < ehdr->e_phnum; i++)
	{
		if (phdr[i].p_type != PT_LOAD)
		{
			continue;
		}

		memcpy((void*)phdr[i].p_paddr,
			(void*)((uintptr_t)ehdr + phdr[i].p_offset),
			phdr[i].p_filesz);
	}

	writeback_dcache();
	invalidate_icache();

	__asm__("j %0" ::"r"(ep) : "memory");

	while (1)
		SleepThread();
	;

	return 0;
}
