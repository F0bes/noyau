// SPDX-License-Identifier: MPL-2.0

#pragma once

namespace sif
{
	enum STATUS
	{
		SIF_STATUS_SIF_INIT = 0x10000,
		SIF_STATUS_CMD_INIT = 0x20000,
		SIF_STATUS_BOOT_END = 0x40000,
	};

	enum COMMAND
	{
		SIF_CMD_SYS = 0x80000000,
		SIF_CMD_SYS_CHANGE_SADDR = SIF_CMD_SYS,
		SIF_CMD_SYS_SREG,
		SIF_CMD_SYS_INIT,
		SIF_CMD_SYS_RESET,
		SIF_CMD_RPC_END = 0x80000000 | 8,
		SIF_CMD_RPC_BIND = 0x80000000 | 9,
		SIF_CMD_RPC_CALL = 0x80000000 | 10,
	};

	// Taken from ps2tek
	enum SERVERS
	{
		SIF_SERVER_SYS_FILEIO = 0x80000001,
		SIF_SERVER_SYS_IOPHEAP = 0x80000003,
		SIF_SERVER_SYS_LOADFILE = 0x80000006,
		SIF_SERVER_SYS_PADMAN = 0x80000100,
		SIF_SERVER_SYS_PADMAN_EX = 0x80000101,
		SIF_SERVER_SYS_MCSERV = 0x80000400,
		SIF_SERVER_SYS_CDVD_INIT = 0x80000592,
		SIF_SERVER_SYS_CDVD_S = 0x80000593,
		SIF_SERVER_SYS_CDVD_N = 0x80000595,
		SIF_SERVER_SYS_CDVD_SEARCH = 0x80000597,
		SIF_SERVER_SYS_CDVD_READY = 0x8000059A,
		SIF_SERVER_SYS_LIBSD = 0x80000701,
		SIF_SERVER_SYS_MTAP_OPEN = 0x80000901,
		SIF_SERVER_SYS_MTAP_CLOSE = 0x80000902,
		SIF_SERVER_SYS_MTAP_GET = 0x80000903,
		SIF_SERVER_SYS_MTAP_UNKN = 0x80000904,
		SIF_SERVER_SYS_MTAP_UNKN2 = 0x80000905,
		SIF_SERVER_SYS_EYETOY = 0x80001400
	};

	struct sif_rpc_client_t
	{
		u32 server;
		u32 server_buf;

		u32 client_size;
		void* client_buf;

		volatile bool ready;
	} STRUCT_ALIGNED;

	constexpr u32 SYS_ID_TO_ID(u32 id) { return id & ~SIF_CMD_SYS; }

	void sif_init();
	s32 sif_bind(sif_rpc_client_t* client, u32 server_id);
	s32 sif_call(sif_rpc_client_t* client, u32 rpc_id, const void* send_buf, size_t send_size, void* recv_buf, u32 recv_size);
} // namespace sif
