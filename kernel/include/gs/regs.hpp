// SPDX-License-Identifier: MPL-2.0

#pragma once

namespace gs
{
	enum GIF_REG
	{
		GIF_REG_PRIM = 0x00,
		GIF_REG_RGBA = 0x01,
		GIF_REG_STQ = 0x02,
		GIF_REG_UV = 0x03,
		GIF_REG_XYZF2 = 0x04,
		GIF_REG_XYZ2 = 0x05,
		GIF_REG_TEX0_1 = 0x06,
		GIF_REG_TEX0_2 = 0x07,
		GIF_REG_CLAMP_1 = 0x08,
		GIF_REG_CLAMP_2 = 0x09,
		GIF_REG_FOG = 0x0a,
		GIF_REG_INVALID = 0x0b,
		GIF_REG_XYZF3 = 0x0c,
		GIF_REG_XYZ3 = 0x0d,
		GIF_REG_A_D = 0x0e,
		GIF_REG_NOP = 0x0f,
	};

	enum GIF_A_D_REG
	{
		GIF_A_D_REG_PRIM = 0x00,
		GIF_A_D_REG_RGBAQ = 0x01,
		GIF_A_D_REG_ST = 0x02,
		GIF_A_D_REG_UV = 0x03,
		GIF_A_D_REG_XYZF2 = 0x04,
		GIF_A_D_REG_XYZ2 = 0x05,
		GIF_A_D_REG_TEX0_1 = 0x06,
		GIF_A_D_REG_TEX0_2 = 0x07,
		GIF_A_D_REG_CLAMP_1 = 0x08,
		GIF_A_D_REG_CLAMP_2 = 0x09,
		GIF_A_D_REG_FOG = 0x0a,
		GIF_A_D_REG_XYZF3 = 0x0c,
		GIF_A_D_REG_XYZ3 = 0x0d,
		GIF_A_D_REG_NOP = 0x0f,
		GIF_A_D_REG_TEX1_1 = 0x14,
		GIF_A_D_REG_TEX1_2 = 0x15,
		GIF_A_D_REG_TEX2_1 = 0x16,
		GIF_A_D_REG_TEX2_2 = 0x17,
		GIF_A_D_REG_XYOFFSET_1 = 0x18,
		GIF_A_D_REG_XYOFFSET_2 = 0x19,
		GIF_A_D_REG_PRMODECONT = 0x1a,
		GIF_A_D_REG_PRMODE = 0x1b,
		GIF_A_D_REG_TEXCLUT = 0x1c,
		GIF_A_D_REG_SCANMSK = 0x22,
		GIF_A_D_REG_MIPTBP1_1 = 0x34,
		GIF_A_D_REG_MIPTBP1_2 = 0x35,
		GIF_A_D_REG_MIPTBP2_1 = 0x36,
		GIF_A_D_REG_MIPTBP2_2 = 0x37,
		GIF_A_D_REG_TEXA = 0x3b,
		GIF_A_D_REG_FOGCOL = 0x3d,
		GIF_A_D_REG_TEXFLUSH = 0x3f,
		GIF_A_D_REG_SCISSOR_1 = 0x40,
		GIF_A_D_REG_SCISSOR_2 = 0x41,
		GIF_A_D_REG_ALPHA_1 = 0x42,
		GIF_A_D_REG_ALPHA_2 = 0x43,
		GIF_A_D_REG_DIMX = 0x44,
		GIF_A_D_REG_DTHE = 0x45,
		GIF_A_D_REG_COLCLAMP = 0x46,
		GIF_A_D_REG_TEST_1 = 0x47,
		GIF_A_D_REG_TEST_2 = 0x48,
		GIF_A_D_REG_PABE = 0x49,
		GIF_A_D_REG_FBA_1 = 0x4a,
		GIF_A_D_REG_FBA_2 = 0x4b,
		GIF_A_D_REG_FRAME_1 = 0x4c,
		GIF_A_D_REG_FRAME_2 = 0x4d,
		GIF_A_D_REG_ZBUF_1 = 0x4e,
		GIF_A_D_REG_ZBUF_2 = 0x4f,
		GIF_A_D_REG_BITBLTBUF = 0x50,
		GIF_A_D_REG_TRXPOS = 0x51,
		GIF_A_D_REG_TRXREG = 0x52,
		GIF_A_D_REG_TRXDIR = 0x53,
		GIF_A_D_REG_HWREG = 0x54,
		GIF_A_D_REG_SIGNAL = 0x60,
		GIF_A_D_REG_FINISH = 0x61,
		GIF_A_D_REG_LABEL = 0x62,
	};

	union alpha_t
	{
		struct
		{
			u32 R : 2;
			u32 G : 2;
			u32 B : 2;
			u32 A : 2;
			u32 : 24;
			u32 FIX : 8;
		} __attribute((packed));
		u64 m64;
	};

	union bitbltbuf_t
	{
		struct
		{
			u32 SBP : 14;
			u32 : 2;
			u32 SBW : 6;
			u32 : 2;
			u32 SBPSM : 6;
			u32 : 2;
			u32 DBP : 14;
			u32 : 2;
			u32 DBW : 6;
			u32 : 2;
			u32 DBPSM : 6;
		} __attribute((packed));
		u64 m64;
	};

	union clamp_t
	{
		struct
		{
			u32 WMS : 2;
			u32 WMT : 2;
			u32 MINU : 10;
			u32 MAXU : 10;
			u32 MINV : 10;
			u32 MAXV : 10;
		} __attribute((packed));
		u64 m64;
	};

	union colclamp_t
	{
		struct
		{
			u32 CLAMP : 1;
		} __attribute((packed));
		u64 m64;
	};

	union dimx_t
	{
		struct
		{
			u32 DM00 : 3;
			u32 : 1;
			u32 DM01 : 3;
			u32 : 1;
			u32 DM02 : 3;
			u32 : 1;
			u32 DM03 : 3;
			u32 : 1;
			u32 DM10 : 3;
			u32 : 1;
			u32 DM11 : 3;
			u32 : 1;
			u32 DM12 : 3;
			u32 : 1;
			u32 DM13 : 3;
			u32 : 1;
			u32 DM20 : 3;
			u32 : 1;
			u32 DM21 : 3;
			u32 : 1;
			u32 DM22 : 3;
			u32 : 1;
			u32 DM23 : 3;
			u32 : 1;
			u32 DM30 : 3;
			u32 : 1;
			u32 DM31 : 3;
			u32 : 1;
			u32 DM32 : 3;
		} __attribute((packed));
		u64 m64;
	};


	union dthe_t
	{
		struct
		{
			u32 DTH_E : 1;
		} __attribute((packed));
		u64 m64;
	};


	union fba_t
	{
		struct
		{
			u32 FBA_E : 1;
		} __attribute((packed));
		u64 m64;
	};

	union finish_t
	{
		struct
		{
			u64 DATA;
		} __attribute((packed));
		u64 m64;
	};

	union fog_t
	{
		struct
		{
			u64 : 56;
			u32 F : 8;
		} __attribute((packed));
		u64 m64;
	};

	union fogcol_t
	{
		struct
		{
			u32 FCR : 8;
			u32 FBG : 8;
			u32 FCB : 8;
		} __attribute((packed));
		u64 m64;
	};


	union frame_t
	{
		struct
		{
			u32 FBP : 9;
			u32 : 7;
			u32 FBW : 6;
			u32 : 2;
			u32 PSM : 6;
			u32 : 2;
			u32 FBMSK : 32;
		} __attribute((packed));
		u64 m64;
	};


	union hwreg_t
	{
		struct
		{
			u64 DATA : 64;
		} __attribute((packed));
		u64 m64;
	};


	union label_t
	{
		struct
		{
			u32 ID : 32;
			u32 IDMSK : 32;
		} __attribute((packed));
		u64 m64;
	};


	union miptbp1_t
	{
		struct
		{
			u32 TB1 : 14;
			u32 TBW1 : 6;
			u32 TBP2 : 14;
			u32 TBW2 : 6;
			u32 TBP3 : 14;
			u32 TBW3 : 6;
		} __attribute((packed));
		u64 m64;
	};


	union miptbp2_t
	{
		struct
		{
			u32 TBP4 : 14;
			u32 TBW4 : 6;
			u32 TBP5 : 14;
			u32 TBW5 : 6;
			u32 TBP6 : 14;
			u32 TBW6 : 6;
		} __attribute((packed));
		u64 m64;
	};

	union pabe_t
	{
		struct
		{
			u32 PAB_E : 1;
		} __attribute((packed));
		u64 m64;
	};

	union prim_t
	{
		struct
		{
			u32 PRI : 3;
			u32 IIP : 1;
			u32 TME : 1;
			u32 FGE : 1;
			u32 ABE : 1;
			u32 AA1 : 1;
			u32 FST : 1;
			u32 CTXT : 1;
			u32 FIX : 1;
		} __attribute((packed));
		u64 m64;
	};


	union prmode_t
	{
		struct
		{
			u32 : 3;
			u32 IIP : 1;
			u32 TME : 1;
			u32 FGE : 1;
			u32 ABE : 1;
			u32 AA1 : 1;
			u32 FST : 1;
			u32 CTXT : 1;
			u32 FIX : 1;
		} __attribute((packed));
		u64 m64;
	};

	union prmodecont_t
	{
		struct
		{
			u32 AC : 1;
		} __attribute((packed));
		u64 m64;
	};


	union rgbaq_t
	{
		struct
		{
			u32 R : 8;
			u32 G : 8;
			u32 B : 8;
			u32 A : 8;
			u32 Q : 32;
		} __attribute((packed));
		u64 m64;
	};


	union scnmsk_t
	{
		struct
		{
			u32 MSK : 32;
		} __attribute((packed));
		u64 m64;
	};


	union scissor_t
	{
		struct
		{
			u32 SCAX0 : 11;
			u32 : 5;
			u32 SCAX1 : 11;
			u32 : 5;
			u32 SCAY0 : 11;
			u32 : 5;
			u32 SCAY1 : 11;
		} __attribute((packed));
		u64 m64;
	};


	union st_t
	{
		struct
		{
			u32 S : 32;
			u32 T : 32;
		} __attribute((packed));
		u64 m64;
	};

	union test_t
	{
		struct
		{
			u32 ATE : 1;
			u32 ATST : 3;
			u32 AREF : 8;
			u32 AFAIL : 2;
			u32 DATE : 1;
			u32 DATM : 1;
			u32 ZTE : 1;
			u32 ZTST : 2;
		} __attribute((packed));
		u64 m64;
	};


	union tex0_t
	{
		struct
		{
			u32 TBP0 : 14;
			u32 TBW : 6;
			u32 PSM : 6;
			u32 TW : 4;
			u32 TH : 4;
			u32 TCC : 1;
			u32 TFX : 2;
			u32 CBP : 14;
			u32 CPSM : 4;
			u32 CSM : 1;
			u32 CSA : 5;
			u32 CLD : 3;
		} __attribute((packed));
		u64 m64;
	};

	union tex1_t
	{
		struct
		{
			u32 LCM : 1;
			u32 : 1;
			u32 MXL : 3;
			u32 MMAG : 1;
			u32 MMIN : 3;
			u32 MTBA : 1;
			u32 : 9;
			u32 L : 2;
			u32 : 11;
			u32 K : 12;
		} __attribute((packed));
		u64 m64;
	};


	union tex2_t
	{
		struct
		{
			u32 : 20;
			u32 PSM : 6;
			u32 : 6;
			u32 : 5;
			u32 CBP : 14;
			u32 CPSM : 4;
			u32 CSM : 1;
			u32 CSA : 5;
			u32 CLD : 3;
		} __attribute((packed));
		u64 m64;
	};


	union texa_t
	{
		struct
		{
			u32 TA0 : 8;
			u32 : 7;
			u32 AEM : 1;
			u32 : 16;
			u32 TA1 : 8;
		} __attribute((packed));
		u64 m64;
	};


	union texclut_t
	{
		struct
		{
			u32 CBW : 6;
			u32 COU : 6;
			u32 COV : 10;
		} __attribute((packed));
		u64 m64;
	};

	union texflush_t
	{
		struct
		{
			u64 DATA : 64;
		} __attribute((packed));
		u64 m64;
	};


	union trxdir_t
	{
		struct
		{
			u32 XDIR : 2;
		} __attribute((packed));
		u64 m64;
	};


	union trxpos_t
	{
		struct
		{
			u32 SSAX : 11;
			u32 : 5;
			u32 SSAY : 11;
			u32 : 5;
			u32 DSAX : 11;
			u32 : 5;
			u32 DSAY : 11;
			u32 DIR : 1;
		} __attribute((packed));
		u64 m64;
	};


	union trxreg_t
	{
		struct
		{
			u32 RRW : 12;
			u32 : 20;
			u32 RRH : 12;
		} __attribute((packed));
		u64 m64;
	};


	union uv_t
	{
		struct
		{
			u32 U : 14;
			u32 : 2;
			u32 V : 14;
		} __attribute((packed));
		u64 m64;
	};

	union xyoffset_t
	{
		struct
		{
			u32 OFX : 16;
			u32 : 16;
			u32 OFY : 16;
		} __attribute((packed));
		u64 m64;
	};


	union xyz_t
	{
		struct
		{
			u32 X : 16;
			u32 Y : 16;
			u32 Z : 32;
		} __attribute((packed));
		u64 m64;
	};


	union xyzf_t
	{
		struct
		{
			u32 X : 16;
			u32 Y : 16;
			u32 Z : 24;
			u32 F : 8;
		} __attribute((packed));
		u64 m64;
	};


	union zbuf_t
	{
		struct
		{
			u32 ZBP : 9;
			u32 : 15;
			u32 PSM : 4;
			u32 : 4;
			u32 ZMSK : 1;
		} __attribute((packed));
		u64 m64;
	};

	union giftag_t
	{
		struct
		{
			u32 NLOOP : 15;
			u32 EOP : 1;
			u32 : 16;
			u32 : 14;
			u32 PRE : 1;
			u32 PRIM : 11;
			u32 FLG : 2;
			u32 NREG : 4;
			u64 REGS;
		} __attribute((packed));
		u64 m64[2];
		u128 m128;
	};

	constexpr void pack_giftag(qword_t*& q, const giftag_t& giftag)
	{
		q->d[0] = giftag.m64[0];
		q->d[1] = giftag.m64[1];
		q++;
	};

	constexpr void pack_alpha(qword_t*& q, const alpha_t& alpha)
	{
		q->d[0] = alpha.m64;
		q->d[1] = GIF_A_D_REG_ALPHA_1;
		q++;
	};

	constexpr void pack_bitbltbuf(qword_t*& q, const bitbltbuf_t& bitbltbuf)
	{
		q->d[0] = bitbltbuf.m64;
		q->d[1] = GIF_A_D_REG_BITBLTBUF;
		q++;
	};

	constexpr void pack_clamp(qword_t*& q, const clamp_t& clamp)
	{
		q->d[0] = clamp.m64;
		q->d[1] = GIF_A_D_REG_CLAMP_1;
		q++;
	};

	constexpr void pack_colclamp(qword_t*& q, const colclamp_t& colclamp)
	{
		q->d[0] = colclamp.m64;
		q->d[1] = GIF_A_D_REG_COLCLAMP;
		q++;
	};

	constexpr void pack_dimx(qword_t*& q, const dimx_t& dimx)
	{
		q->d[0] = dimx.m64;
		q->d[1] = GIF_A_D_REG_DIMX;
		q++;
	};

	constexpr void pack_dthe(qword_t*& q, const dthe_t& dthe)
	{
		q->d[0] = dthe.m64;
		q->d[1] = GIF_A_D_REG_DTHE;
		q++;
	};

	constexpr void pack_fba(qword_t*& q, const fba_t& fba)
	{
		q->d[0] = fba.m64;
		q->d[1] = GIF_A_D_REG_FBA_1;
		q++;
	};

	constexpr void pack_finish(qword_t*& q, const finish_t& finish)
	{
		q->d[0] = finish.m64;
		q->d[1] = GIF_A_D_REG_FINISH;
		q++;
	};

	constexpr void pack_fog(qword_t*& q, const fog_t& fog)
	{
		q->d[0] = fog.m64;
		q->d[1] = GIF_A_D_REG_FOG;
		q++;
	};

	constexpr void pack_fogcol(qword_t*& q, const fogcol_t& fogcol)
	{
		q->d[0] = fogcol.m64;
		q->d[1] = GIF_A_D_REG_FOGCOL;
		q++;
	};

	constexpr void pack_frame(qword_t*& q, const frame_t& frame)
	{
		q->d[0] = frame.m64;
		q->d[1] = GIF_A_D_REG_FRAME_1;
		q++;
	};

	constexpr void pack_hwreg(qword_t*& q, const hwreg_t& hwreg)
	{
		q->d[0] = hwreg.m64;
		q->d[1] = GIF_A_D_REG_HWREG;
		q++;
	};

	constexpr void pack_label(qword_t*& q, const label_t& label)
	{
		q->d[0] = label.m64;
		q->d[1] = GIF_A_D_REG_LABEL;
		q++;
	};

	constexpr void pack_miptbp1(qword_t*& q, const miptbp1_t& miptbp1)
	{
		q->d[0] = miptbp1.m64;
		q->d[1] = GIF_A_D_REG_MIPTBP1_1;
		q++;
	};

	constexpr void pack_miptbp2(qword_t*& q, const miptbp2_t& miptbp2)
	{
		q->d[0] = miptbp2.m64;
		q->d[1] = GIF_A_D_REG_MIPTBP2_1;
		q++;
	};

	constexpr void pack_pabe(qword_t*& q, const pabe_t& pabe)
	{
		q->d[0] = pabe.m64;
		q->d[1] = GIF_A_D_REG_PABE;
		q++;
	};

	constexpr void pack_prim(qword_t*& q, const prim_t& prim)
	{
		q->d[0] = prim.m64;
		q->d[1] = GIF_A_D_REG_PRIM;
		q++;
	};

	constexpr void pack_prmode(qword_t*& q, const prmode_t& prmode)
	{
		q->d[0] = prmode.m64;
		q->d[1] = GIF_A_D_REG_PRMODE;
		q++;
	};

	constexpr void pack_prmodecont(qword_t*& q, const prmodecont_t& prmodecont)
	{
		q->d[0] = prmodecont.m64;
		q->d[1] = GIF_A_D_REG_PRMODECONT;
		q++;
	};

	constexpr void pack_rgbaq(qword_t*& q, const rgbaq_t& rgbaq)
	{
		q->d[0] = rgbaq.m64;
		q->d[1] = GIF_A_D_REG_RGBAQ;
		q++;
	};

	constexpr void pack_scnmsk(qword_t*& q, const scnmsk_t& scnmsk)
	{
		q->d[0] = scnmsk.m64;
		q->d[1] = GIF_A_D_REG_SCANMSK;
		q++;
	};

	constexpr void pack_scissor(qword_t*& q, const scissor_t& scissor)
	{
		q->d[0] = scissor.m64;
		q->d[1] = GIF_A_D_REG_SCISSOR_1;
		q++;
	};

	constexpr void pack_st(qword_t*& q, const st_t& st)
	{
		q->d[0] = st.m64;
		q->d[1] = GIF_A_D_REG_ST;
		q++;
	};

	constexpr void pack_test(qword_t*& q, const test_t& test)
	{
		q->d[0] = test.m64;
		q->d[1] = GIF_A_D_REG_TEST_1;
		q++;
	};

	constexpr void pack_tex0(qword_t*& q, const tex0_t& tex0)
	{
		q->d[0] = tex0.m64;
		q->d[1] = GIF_A_D_REG_TEX0_1;
		q++;
	};

	constexpr void pack_tex1(qword_t*& q, const tex1_t& tex1)
	{
		q->d[0] = tex1.m64;
		q->d[1] = GIF_A_D_REG_TEX1_1;
		q++;
	};

	constexpr void pack_tex2(qword_t*& q, const tex2_t& tex2)
	{
		q->d[0] = tex2.m64;
		q->d[1] = GIF_A_D_REG_TEX2_1;
		q++;
	};

	constexpr void pack_texa(qword_t*& q, const texa_t& texa)
	{
		q->d[0] = texa.m64;
		q->d[1] = GIF_A_D_REG_TEXA;
		q++;
	};

	constexpr void pack_texclut(qword_t*& q, const texclut_t& texclut)
	{
		q->d[0] = texclut.m64;
		q->d[1] = GIF_A_D_REG_TEXCLUT;
		q++;
	};

	constexpr void pack_texflush(qword_t*& q, const texflush_t& texflush)
	{
		q->d[0] = texflush.m64;
		q->d[1] = GIF_A_D_REG_TEXFLUSH;
		q++;
	};

	constexpr void pack_trxdir(qword_t*& q, const trxdir_t& trxdir)
	{
		q->d[0] = trxdir.m64;
		q->d[1] = GIF_A_D_REG_TRXDIR;
		q++;
	};

	constexpr void pack_trxpos(qword_t*& q, const trxpos_t& trxpos)
	{
		q->d[0] = trxpos.m64;
		q->d[1] = GIF_A_D_REG_TRXPOS;
		q++;
	};

	constexpr void pack_trxreg(qword_t*& q, const trxreg_t& trxreg)
	{
		q->d[0] = trxreg.m64;
		q->d[1] = GIF_A_D_REG_TRXREG;
		q++;
	};

	constexpr void pack_uv(qword_t*& q, const uv_t& uv)
	{
		q->d[0] = uv.m64;
		q->d[1] = GIF_A_D_REG_UV;
		q++;
	};

	constexpr void pack_xyz(qword_t*& q, const xyz_t& xyz)
	{
		q->d[0] = xyz.m64;
		q->d[1] = GIF_A_D_REG_XYZ2;
		q++;
	};

	constexpr void pack_xyzf(qword_t*& q, const xyzf_t& xyzf)
	{
		q->d[0] = xyzf.m64;
		q->d[1] = GIF_A_D_REG_XYZF2;
		q++;
	};

	constexpr void pack_xyoffset(qword_t*& q, const xyoffset_t& xyoffset)
	{
		q->d[0] = xyoffset.m64;
		q->d[1] = GIF_A_D_REG_XYOFFSET_1;
		q++;
	};

	constexpr void pack_zbuf(qword_t*& q, const zbuf_t& zbuf)
	{
		q->d[0] = zbuf.m64;
		q->d[1] = GIF_A_D_REG_ZBUF_1;
		q++;
	};
}; // namespace gs
