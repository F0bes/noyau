// SPDX-License-Identifier: MPL-2.0

#pragma once

#define DMATAG(QWC, PCE, ID, IRQ, ADDR, SPR) \
	(u64)((QWC) & 0x0000FFFF) << 0 | (u64)((PCE) & 0x00000003) << 26 | \
		(u64)((ID) & 0x00000007) << 28 | (u64)((IRQ) & 0x00000001) << 31 | \
		(u64)((ADDR) & 0x7FFFFFFF) << 32 | (u64)((SPR) & 0x00000001) << 63


enum class DMA_TAG_ID
{
	REFE = 0,
	CNT,
	NEXT,
	REF,
	REFS,
	CALL,
	RET,
	END
};

union dma_tag_t
{
	struct
	{
		u32 qwc : 16;
		u32 : 10;
		u32 pce : 2;
		u32 id : 3;
		u32 irq : 1;
		u32 addr : 31;
		bool spr : 1;
	};
	u64 d;
};

constexpr void dma_pack_refe(qword_t*& q, u32 qwc, ee_ptr_t addr, bool spr, u32 W2, u32 W3)
{
	q->d[0] = dma_tag_t{qwc, 0, static_cast<u32>(DMA_TAG_ID::REFE), 0, addr, spr}.d;
	q->w[2] = W2;
	q->w[3] = W3;
	q++;
}

constexpr void dma_pack_cnt(qword_t*& q, u32 qwc, bool spr, u32 W2, u32 W3)
{
	q->d[0] = dma_tag_t{qwc, 0, static_cast<u32>(DMA_TAG_ID::CNT), 0, 0, spr}.d;
	q->w[2] = W2;
	q->w[3] = W3;
	q++;
}

constexpr void dma_pack_ref(qword_t*& q, u32 qwc, ee_ptr_t addr, bool spr, u32 W2, u32 W3)
{
	q->d[0] = dma_tag_t{qwc, 0, static_cast<u32>(DMA_TAG_ID::REF), 0, addr, spr}.d;
	q->w[2] = W2;
	q->w[3] = W3;
	q++;
}

constexpr void dma_pack_end(qword_t*& q, u32 W2, u32 W3)
{
	q->d[0] = dma_tag_t{0, 0, static_cast<u32>(DMA_TAG_ID::END), 0, 0, 0}.d;
	q->w[2] = W2;
	q->w[3] = W3;
	q++;
}
