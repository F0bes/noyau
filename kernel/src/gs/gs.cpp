// SPDX-License-Identifier: MPL-2.0

#include "gs/gs.hpp"
#include "gs/regs.hpp"
#include "dma/tags.hpp"
#include "mmio/dma.hpp"
#include "mmio/gs.hpp"
#include "debug/debug.hpp"
#include "memutil.hpp"

#include "fs/vfs.hpp"
#include "fs/vdev.hpp"

namespace gs
{
	vdev::vnode_t fb_dev = {
		.name = "/dev/fb",
		.open = nullptr,
		.close = nullptr,
		.read = nullptr, // TODO: GS download??
		.write = [](int fdnum, const void* buf, size_t count) -> int {
			// This is /pretty damn slow/ on a real PS2. Ideally we could write directly to the
			// GIF FIFO. I couldn't get it to work so DMA transfers are good enough
			// for now.
			auto& fd = vfs::get_fd(fdnum);
			const size_t num_pixels = count / 4;

			size_t fb_h = fd.pos / EARLYFB_WIDTH;
			size_t fb_w = fd.pos % EARLYFB_WIDTH;
			qword_t stack_alloc[129];
			qword_t* gif_packet = (qword_t*)_kseg1(stack_alloc);

			size_t dots_left = num_pixels;
			const size_t dots_per_packet = 64;
			while (dots_left)
			{
				qword_t* q = gif_packet;
				const size_t dots_this_packet = dots_left > dots_per_packet ? dots_per_packet : dots_left;

				pack_giftag(q, {dots_this_packet * 2, 1, 1, 0, 0, 1, GIF_REG_A_D});
				for (size_t i = 0; i < dots_this_packet; i++)
				{
					u8* rgba = (u8*)buf;
					pack_rgbaq(q, {rgba[0], rgba[1], rgba[2], rgba[3], 0});
					pack_xyz(q, {fb_w << 4, fb_h << 4, 1});
					buf = (u32*)buf + 1;
					fb_w++;
					if (fb_w >= EARLYFB_WIDTH)
					{
						fb_w = 0;
						fb_h++;
					}
				}

				_s32(GIF_QWC, q - gif_packet);
				_s32(GIF_MADR, _phys(gif_packet));
				_s32(GIF_CHCR, 0x100);
				while (_l32(GIF_CHCR) & 0x100)
					;
				dots_left -= dots_this_packet;
			}

			fd.pos += num_pixels;
			return count;
		},
		.seek = [](int fdnum, s64 offset, int whence) -> int {
			auto& fd = vfs::get_fd(fdnum);
			switch (whence)
			{
				case vfs::SEEK_SET:
					printk("SEEK_SET to %d\n", offset);
					fd.pos = offset;
					break;
				case vfs::SEEK_CUR:
					fd.pos += offset;
					break;
				case vfs::SEEK_END:
					fd.pos = EARLYFB_WIDTH * EARLYFB_HEIGHT + offset;
					break;
			}
			return fd.pos;
		}};

	void fb_init()
	{
		vdev::vdev_register(&fb_dev);
	}

	void gs_init()
	{
		printd("### Initializing GS\n");

		// Stop everything DMA
		_s32(GIF_CHCR, 0);
		_s32(GIF_QWC, 0);

		// Reset the GIF
		_s32(GIF_CTRL, 1);

		// Reset the GS and clear any interrupts
		_s32(GS_CSR, ~0);

		printd("Configuring a %dx%d display\n", EARLYFB_WIDTH, EARLYFB_HEIGHT);

		// Construct the "undefined" registers for NTSC 640x448i
		// Ideally we would have our equivalent of SetGsCrt to handle this
		// For now I'll just hardcode it here

		// I used a reference table from a document by asmblur and ooPo
		// There is a special note about the SINT and PRST registers
		// If we find that some GS revisions don't like this we can try this
		/*
			have_SMODE1_write:
			sd   $a1, Source_SMODE1($s0)         # Source_SMODE1 = a1
			lb   $v0, SMODE1_fix($s2)            # v0 = SMODE1_fix
			beql $v0, $zero, store_v0_as_SMODE1  # in case of SMODE1_fix is disabled
			or   $v0, $zero, $a1                 #   go use v0=a1 for SMODE1
			ld   $v0, Target_SMODE1($s1)         # v0 = Target_SMODE1
			beql $v0, $zero, exit_GSHandler      # in case of  Target_SMODE1 is zero
			sd   $a1, 0($a3)                     #   go use Source_SMODE1
		*/
		/*
	 This SMODE1 is wrong..
		u64 VHP = 0;
		u64 VCKSEL = 1;
		u64 SLCK2 = 1;
		u64 NVCK = 1;
		u64 CLKSEL = 1;
		u64 PEVS = 0;
		u64 PEHS = 0;
		u64 PVS = 0;
		u64 PHS = 0;
		u64 GCONT = 0;
		u64 SPML = 4;
		u64 PCK2 = 0;
		u64 XPCK = 0;
		u64 SINT = 1;
		u64 PRST = 1;
		u64 EX = 0;
		u64 CMOD = 2;
		u64 SLCK = 0;
		u64 T1248 = 1;
		u64 LC = 32;
		u64 RC = 4;

		_s64(GS_SMODE1, 
			((RC & 0x07) << 0) |
			((LC & 0x1F) << 3) |
			((T1248 & 0x03) << 10) |
			((SLCK & 0x01) << 12) |
			((CMOD & 0x03) << 13) |
			((EX & 0x01) << 15) |
			((PRST & 0x01) << 16) |
			((SINT & 0x01) << 17) |
			((XPCK & 0x01) << 18) |
			((PCK2 & 0x01) << 19) |
			((SPML & 0x0F) << 21) |
			((GCONT & 0x01) << 25) |
			((PHS & 0x01) << 26) |
			((PVS & 0x01) << 27) |
			((PEHS & 0x01) << 28) |
			((PEVS & 0x01) << 29) |
			((CLKSEL & 0x03) << 30) |
			((NVCK & 0x01) << 32) |
			((SLCK2 & 0x01) << 33) |
			((VCKSEL & 0x03) << 34) |
			((VHP & 0x01) << 36));
		*/
		_s64(GS_SMODE1, 0x742814504);

		// Interlace mode
		_s64(GS_SMODE2, 0x1);

		// DRAM refresh, correlated to SPML
		_s64(GS_SRFSH, 0x8);

		// Taken from homebrew running on PCSX2
		_s64(GS_SYNCH1, 0x7F5B61F06F040);
		_s64(GS_SYNCH2, 0x33A4D8);
		_s64(GS_SYNCV, 0xC7800601A01801);
		// These ones I can calculate, but MAGH and MAGV is annoying
		_s64(GS_DISPLAY1, 0x1BF9FF0183228C);
		_s64(GS_DISPLAY2, 0x1BF9FF0183228C);

		// FBPTR 0
		// FBW 10 (640)
		// PSM 0
		_s64(GS_DISPFB1, 0x80000009400);
		_s64(GS_DISPFB2, 0x80000009400);

		_s64(GS_PMODE, 0x7027);

		_s64(GS_BGCOLOR, 0x00);

		qword_t* gif_packet = (qword_t*)_kseg1(new qword_t[20]);
		qword_t* q = gif_packet;
		pack_giftag(q, {15, 1, 1, 0x06, 0, 1, GIF_REG_A_D});
		pack_frame(q, {0, 640 >> 6, 1, 0});
		pack_zbuf(q, {0, 0, 1}); // Mask Z, we don't need it
		pack_test(q, {0, 0, 0, 0, 0, 0, 1, 1});
		pack_alpha(q, {0, 1, 0, 1, 128});
		pack_prmodecont(q, {1});
		pack_scissor(q, {0, 639, 0, 447});
		pack_dthe(q, {0});
		pack_xyoffset(q, {0, 0});
		pack_scnmsk(q, {0});
		pack_clamp(q, {0, 0, 0, 0, 0, 0});
		pack_pabe(q, {0});
		pack_fba(q, {0});
		pack_rgbaq(q, {0, 0x80, 0, 1, 0});
		pack_xyz(q, {0, 0, 0});
		pack_xyz(q, {640 << 4, 448 << 4, 0});

		_s32(GIF_QWC, q - gif_packet);
		_s32(GIF_MADR, _phys(gif_packet));
		printk("Kicking off a GIf packet. ptr %p, size %d qw\n", _phys(gif_packet), q - gif_packet);
		_s32(GIF_CHCR, 0x101);

		while (_l32(GIF_CHCR) & 0x100)
			;
		printk("### GS Display Initialized\n");
		delete[] gif_packet;

		printk("### GS Initialized\n");

		fb_init();
		vterm_init();
	}

	// data/vterm_font_itex.c
	extern "C" u32 size_vterm_font_itex;
	extern "C" u8 vterm_font_itex[];

	void vterm_reset()
	{
	}

	void vterm_init()
	{
		// Upload the font itex
		qword_t* gif_packet = (qword_t*)_kseg1(new qword_t[20]);
		qword_t* q = gif_packet;

		const u32 tex_qword_cnt = (size_vterm_font_itex / 16) + 1;

		dma_pack_cnt(q, 6, 0, 0, 0);
		printk("DMATag %08X %08X %08X %08X", q->w[0], q->w[1], q->w[2], q->w[3]);
		pack_giftag(q, {4, 0, 0, 0, 0, 1, GIF_REG_A_D});
		pack_bitbltbuf(q, {0, 0, 0, 0, 10, 0x2C}); // 4HH 0x2C
		pack_trxpos(q, {0, 0, 0, 0, 0});
		pack_trxreg(q, {128, 256});
		pack_trxdir(q, {0});
		pack_giftag(q, {tex_qword_cnt, 0, 0, 0, 2, 0, 0});
		dma_pack_refe(q, tex_qword_cnt, _phys(vterm_font_itex), 0, 0, 0);
		_s32(GIF_QWC, 0);
		_s32(GIF_TADR, _phys(gif_packet));
		_s32(GIF_CHCR, 0x105);

		while (_l32(GIF_CHCR) & 0x100)
			;

		printk("### VTERM Itex Font Uploaded\n");

		// Generate CLUT
		qword_t clut;
		clut.w[0] = 0x7FFFFFFF;
		clut.w[1] = 0x00000000;
		clut.w[2] = 0x7F00FF7F;
		clut.w[3] = 0x7F00FF7F;

		q = gif_packet;
		pack_giftag(q, {5, 0, 0, 0, 0, 1, GIF_REG_A_D});
		pack_bitbltbuf(q, {0, 0, 0, 0x56000 >> 6, 0, 0x00});
		pack_trxpos(q, {0, 0, 0, 0, 0});
		pack_trxreg(q, {16, 1});
		pack_trxdir(q, {0});
		pack_texflush(q, {0});
		pack_giftag(q, {2, 0, 0, 0, 2, 0, 0});
		*q = clut;
		q++;
		*q = clut;
		q++;
		_s32(GIF_QWC, q - gif_packet);
		_s32(GIF_MADR, _phys(gif_packet));
		_s32(GIF_CHCR, 0x101);

		while (_l32(GIF_CHCR) & 0x100)
			;
		printk("### VTERM CLUT Uploaded\n");

		q = gif_packet;

		pack_giftag(q, {7, 1, 1, 0x06 | 1 << 4 | 1 << 8, 0, 1, GIF_REG_A_D});
		pack_tex0(q, {0, 10, 0x2C, 7, 8, 1, 1, 0x56000 >> 6, 0, 1, 0, 1});
		pack_test(q, {1, 7, 0, 0, 0, 0, 1, 1});
		pack_rgbaq(q, {0x80, 0x80, 0, 0xFF, 0x3f800000});
		pack_uv(q, {0, 0});
		pack_xyz(q, {0, 0, 1});
		pack_uv(q, {128 << 4, 256 << 4});
		pack_xyz(q, {128 << 4, 256 << 4, 1});

		_s32(GIF_QWC, q - gif_packet);
		_s32(GIF_MADR, _phys(gif_packet));
		_s32(GIF_CHCR, 0x101);

		while (_l32(GIF_CHCR) & 0x100)
			;

		printk("### VTERM Font Displayed\n");

		delete[] gif_packet;
	}
} // namespace gs
