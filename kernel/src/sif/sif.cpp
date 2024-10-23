// SPDX-License-Identifier: MPL-2.0

#include "sif/sif.hpp"
#include "mmio/sif.hpp"
#include "mmio/dma.hpp"
#include "memutil.hpp"
#include "util/util.hpp"
#include "debug/debug.hpp"
#include "intc/intc.hpp"

namespace sif
{
	static u8* sif0_buf;
	static u8* sif1_buf;
	static iop_ptr_t iop_smcom;
	static u32 sys_regs[32] = {0};

	struct sif_reset_pkt_t
	{
		u32 argsize;
		u32 mode;
		u8 arg[80];
	} STRUCT_ALIGNED_PACKED;

	struct sif_cmd_header_t
	{
		u32 pkt_size : 8;
		u32 data_size : 24;
		u32 data_addr;
		u32 cmd;
		u32 opt;
	} STRUCT_PACKED;

	struct sif_iop_dmatag_t
	{
		u32 addr : 24;
		u32 : 6;
		u32 ts1 : 1; // unknown
		u32 ts2 : 1; // unknown
		u32 wc;
		u64 : 64;
	} STRUCT_ALIGNED_PACKED;

	typedef void (*sif_cmd_handler_t)(const sif_cmd_header_t* header);

	constexpr size_t SIF_SYS_CMDS_MAX = 16;

	sif_cmd_handler_t sif_sys_cmd_handlers[SIF_SYS_CMDS_MAX] = {nullptr};

	void sif_dma_shutdown()
	{
		_s32(SIF0_CHCR, 0);
		_s32(SIF0_QWC, 0);
		_s32(SIF0_MADR, 0);
		_l32(SIF0_QWC);

		_s32(SIF1_CHCR, 0);
	}

	void sif1_write(const sif_cmd_header_t* header, bool ert, bool irq, iop_ptr_t dst, const void* src, size_t size)
	{
		const size_t size_aligned = (((header ? sizeof(sif_cmd_header_t) : 0) + size) & ~0xF) + 0x10;
		const sif_iop_dmatag_t tag = {dst, irq, ert, size_aligned / 4};
		const size_t dma_size = sizeof(sif_iop_dmatag_t) + size_aligned;

		if (!size_aligned || !header)
			return;

		u8* dma_buf = sif1_buf;

		memcpy(dma_buf, &tag, sizeof(tag));
		memcpy(dma_buf + sizeof(tag), header, sizeof(sif_cmd_header_t));
		memcpy(dma_buf + sizeof(tag) + sizeof(sif_cmd_header_t), src, size);

		_s32(SIF1_MADR, _phys(dma_buf));

		_s32(SIF1_QWC, dma_size / 16);
		_s32(SIF1_CHCR, 0x181);
	}

	void sif_cmd(u32 cmd, u32 opt, const void* pkt, size_t pkt_size, iop_ptr_t dst, const void* src, size_t size)
	{
		const sif_cmd_header_t header = {sizeof(sif_cmd_header_t) + pkt_size, size, dst, cmd, opt};

		sif1_write(&header, true, true, iop_smcom, pkt, pkt_size);
	}

	void sif_iop_reset()
	{
		// Should argsize be 1 or 0?
		sif_reset_pkt_t pkt = {1, 0, {0}};

		_s32(SIF_SMFLG, SIF_STATUS_BOOT_END);

		sif_cmd(SIF_CMD_SYS_RESET, 0, &pkt, sizeof(pkt), nullptr, nullptr, 0);

		_s32(SIF_SMFLG, SIF_STATUS_SIF_INIT | SIF_STATUS_CMD_INIT);

		printk("IOP Reset. Waiting for IOP to boot...\n");
		while (!(_l32(SIF_SMFLG) & SIF_STATUS_BOOT_END))
			;
		printk("IOP Booted\n");
	}

	s32 sif0_handler_id = -1;
	intc::handler_fun_t sif0_handler = [](u32 cause) {
		if ((_l32(SIF0_CHCR) & 0x100))
		{
			printw("SIF0 DMA interrupted but not completed\n");
			return;
		}

		const sif_cmd_header_t* header = reinterpret_cast<sif_cmd_header_t*>(sif0_buf);

		if (header->cmd & SIF_CMD_SYS)
		{
			const u32 cmd = header->cmd & ~SIF_CMD_SYS;
			if (cmd > SIF_SYS_CMDS_MAX)
			{
				printw("SIF0 SIF SYS unknown command 0x%08X\n", cmd);
				return;
			}

			if (!sif_sys_cmd_handlers[cmd])
			{
				printw("SIF0 SIF SYS command 0x%08X not implemented\n", cmd);
				return;
			}

			sif_sys_cmd_handlers[cmd](header);
		}
		else
		{
			printw("SIF0 SIF USR command 0x%08X not implemented\n", header->cmd);
		}
	};

	void sif_handlers_install()
	{
		sif_sys_cmd_handlers[SYS_ID_TO_ID(SIF_CMD_SYS_CHANGE_SADDR)] = [](const sif_cmd_header_t* header) {
			printw("SIF0 SIF SYS Change SADDR\n");
		};

		sif_sys_cmd_handlers[SYS_ID_TO_ID(SIF_CMD_SYS_SREG)] = [](const sif_cmd_header_t* header) {
			const struct
			{
				u32 reg;
				u32 val;
			}* set_sysreg = reinterpret_cast<decltype(set_sysreg)>(header + 1);

			printd("SIF0 SIF SET SREG %08X = %08X\n", set_sysreg->reg, set_sysreg->val);

			sys_regs[set_sysreg->reg] = set_sysreg->val;
		};

		sif_sys_cmd_handlers[SYS_ID_TO_ID(SIF_CMD_SYS_INIT)] = [](const sif_cmd_header_t* header) {
			printw("SIF0 SIF SYS Init\n");
		};

		sif_sys_cmd_handlers[SYS_ID_TO_ID(SIF_CMD_SYS_RESET)] = [](const sif_cmd_header_t* header) {
			printw("SIF0 SIF SYS Reset\n");
		};
	}

	void sif_init()
	{
		sif_dma_shutdown();
		sif0_buf = new u8[0x1000];
		sif1_buf = new u8[0x1000];

		// Retrieve the IOP's dma buffer
		iop_smcom = _l32(SIF_SMCOM);

		// Load our kernels dma buffer
		_s32(SIF_F260, 0xFF);
		_s32(SIF_MSCOM, _phys(sif0_buf));
		_s32(SIF_MSFLG, SIF_STATUS_CMD_INIT | SIF_STATUS_BOOT_END);

		sif_iop_reset();

		_s32(SIF_F260, 0xFF);
		_s32(SIF_MSCOM, _phys(sif0_buf));
		_s32(SIF_MSFLG, SIF_STATUS_CMD_INIT | SIF_STATUS_BOOT_END);

		iop_smcom = _l32(SIF_SMCOM);

		sif_handlers_install();

		if (sif0_handler_id == -1)
			sif0_handler_id = intc::register_dmac_handler(intc::DMAC_CAUSE::SIF0_DMA, sif0_handler, true);

		intc::enable_dmac_interrupt(intc::DMAC_CAUSE::SIF0_DMA);

		_s32(SIF0_QWC, 0);
		_s32(SIF0_MADR, 0);
		_s32(SIF0_CHCR, 0x184);

		const u32 sif0_buf_ptr = _phys(sif0_buf);
		sif_cmd(SIF_CMD_SYS_INIT, 0, &sif0_buf_ptr, sizeof(u32), nullptr, nullptr, 0);

		sif_cmd(SIF_CMD_SYS_INIT, 1, nullptr, 0, nullptr, nullptr, 0);

		printk("SIF RPC Init. Waiting for IOP to respond...\n");

		while (sys_regs[0] == 0)
			;

		printk("IOP responded. RPC is alive\n");
	}
} // namespace sif
