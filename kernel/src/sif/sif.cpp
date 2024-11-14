// SPDX-License-Identifier: MPL-2.0

#include "sif/sif.hpp"
#include "mmio/sif.hpp"
#include "mmio/dma.hpp"
#include "memutil.hpp"
#include "memory.hpp"
#include "util/util.hpp"
#include "debug/debug.hpp"
#include "intc/intc.hpp"

namespace sif
{
	static u8* sif0_buf;
	static u8* sif1_buf;
	constexpr u32 sif_buffer_size = 0x1000;

	static iop_ptr_t iop_smcom;
	static u32 sys_regs[32] = {0};

	struct sif_rpc_pkt_header_t
	{
		s32 rec_id;
		void* pkt_addr;
		s32 rpc_id;
	};

	struct sif_bind_pkt_t
	{
		sif_rpc_pkt_header_t header;
		sif_rpc_client_t* client;
		u32 server_id;
	} STRUCT_ALIGNED_PACKED;

	struct sif_rpc_end_pkt_t
	{
		sif_rpc_pkt_header_t header;
		sif_rpc_client_t* client;
		u32 client_id;

		u32 server;
		u32 server_buf;
		void* client_buf;
	} STRUCT_ALIGNED_PACKED;

	struct sif_rpc_call_pkt_t
	{
		sif_rpc_pkt_header_t header;
		sif_rpc_client_t* client;
		u32 rpc_id;

		u32 send_size;
		u32 recv_addr;
		u32 recv_size;
		u32 recv_mode;

		u32 server;
	} STRUCT_ALIGNED_PACKED;

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
	} STRUCT_ALIGNED_PACKED;

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
		const size_t header_size = header ? sizeof(*header) : 0;
		const size_t aligned_size = ((header_size + size) & ~0xF) + 0x10;
		const sif_iop_dmatag_t tag = {dst, irq, ert, aligned_size / 4};
		const size_t dma_size = sizeof(sif_iop_dmatag_t) + aligned_size;

		if (!aligned_size)
			return;

		u8* dma_buf = sif1_buf;

		memcpy(dma_buf, &tag, sizeof(tag));
		memcpy(dma_buf + sizeof(tag), header, header_size);
		memcpy(dma_buf + sizeof(tag) + header_size, src, size);

		if (_l32(SIF1_CHCR) & 0x100)
		{
			printw("SIF1 DMA already in progress, waiting\n");
		}

		for (int i = 0; i < 2000000; i++)
		{
			if (!(_l32(SIF1_CHCR) & 0x100))
				break;
		}
		if (_l32(SIF1_CHCR) & 0x100)
		{
			printe("SIF1 DMA still in progress, aborting\n");
			return;
		}

		_s32(SIF1_MADR, _phys(dma_buf));

		_s32(SIF1_QWC, dma_size / 16);
		_s32(SIF1_CHCR, 0x181);
	}

	void sif_cmd(u32 cmd, u32 opt, const void* pkt, size_t pkt_size, iop_ptr_t dst, const void* src, size_t size)
	{
		const sif_cmd_header_t header = {sizeof(sif_cmd_header_t) + pkt_size, size, dst, cmd, opt};
		// Kick data
		sif1_write(NULL, false, false, dst, src, size);
		// Kick header/pkt
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
		printi("SIF0 DMA Interrupt!\n");
		_s32(D_STAT, 1 << 5);
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

		_s32(SIF0_MADR, 0);
		_s32(SIF0_QWC, 0);
		_s32(SIF0_CHCR, 0x184);
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

		sif_sys_cmd_handlers[SYS_ID_TO_ID(SIF_CMD_RPC_END)] = [](const sif_cmd_header_t* header) {
			const sif_rpc_end_pkt_t* pkt = reinterpret_cast<const sif_rpc_end_pkt_t*>(header + 1);

			printd("SIF0 SIF RPC End (client ID %X)\n", pkt->client_id);

			switch (pkt->client_id)
			{
				case SIF_CMD_RPC_CALL:
					break;
				case SIF_CMD_RPC_BIND:
					printd("SIF EE->IOP BIND complete\n");
					pkt->client->server = pkt->server;
					pkt->client->server_buf = pkt->server_buf;
			}
			pkt->client->ready = true;
		};
	}

	void sif_init()
	{
		sif_dma_shutdown();
		sif0_buf = new u8[sif_buffer_size];
		sif1_buf = new u8[sif_buffer_size];

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

		printi("SIF RPC Init. Waiting for IOP to respond...\n");

		while (sys_regs[0] == 0)
			;

		printi("IOP responded. RPC is alive\n");

		// IOP modules take some time to init
		for (int i = 0; i < 30000000; i++)
			asm volatile("nop\nnop\nnop\n");
	}

	s32 sif_bind(sif_rpc_client_t* client, u32 server_id)
	{
		const sif_bind_pkt_t pkt = {
			.header = {0, nullptr, 0},
			.client = client,
			.server_id = server_id,
		};
		client->ready = false;
		client->client_size = sif_buffer_size;
		client->client_buf = kmalloc(sif_buffer_size);
		client->server = 0;
		sif_cmd(SIF_CMD_RPC_BIND, 0, &pkt, sizeof(pkt), nullptr, nullptr, 0);

		while (!client->ready)
			;
		return 0;
	}

	s32 sif_call(sif_rpc_client_t* client, u32 rpc_id, const void* send_buf, size_t send_size, void* recv_buf, u32 recv_size)
	{
		const sif_rpc_call_pkt_t pkt = {
			.header = {0, nullptr, 0},
			.client = client,
			.rpc_id = rpc_id,
			.send_size = send_size,
			.recv_addr = _phys(client->client_buf),
			.recv_size = recv_size,
			.recv_mode = 1,
			.server = client->server,
		};

		client->ready = false;
		if (client->client_size < send_size)
		{
			printw("RPC client buffer too small\n");
			return -1;
		}

		sif_cmd(SIF_CMD_RPC_CALL, 0, &pkt, sizeof(pkt), client->server_buf, send_buf, send_size);
		while (!client->ready)
			;

		memcpy(recv_buf, client->client_buf, recv_size);

		printi("RPC call complete\n");
		return 0;
	}
} // namespace sif
