// SPDX-License-Identifier: MPL-2.0

#pragma once

constexpr u32 CH0_CHCR = 0x10008000;
constexpr u32 CH0_MADR = 0x10008010;
constexpr u32 CH0_QWC =  0x10008020;
constexpr u32 CH0_TADR = 0x10008030;
constexpr u32 CH0_ASR0 = 0x10008040;
constexpr u32 CH0_ASR1 = 0x10008050;

constexpr u32 CH1_CHCR = 0x10009000;
constexpr u32 CH1_MADR = 0x10009010;
constexpr u32 CH1_QWC =  0x10009020;
constexpr u32 CH1_TADR = 0x10009030;
constexpr u32 CH1_ASR0 = 0x10009040;
constexpr u32 CH1_ASR1 = 0x10009050;

constexpr u32 CH2_CHCR = 0x1000A000;
constexpr u32 CH2_MADR = 0x1000A010;
constexpr u32 CH2_QWC =  0x1000A020;
constexpr u32 CH2_TADR = 0x1000A030;
constexpr u32 CH2_ASR0 = 0x1000A040;
constexpr u32 CH2_ASR1 = 0x1000A050;

constexpr u32 CH3_CHCR = 0x1000B000;
constexpr u32 CH3_MADR = 0x1000B010;
constexpr u32 CH3_QWC =  0x1000B020;

constexpr u32 CH4_CHCR = 0x1000B400;
constexpr u32 CH4_MADR = 0x1000B410;
constexpr u32 CH4_QWC =  0x1000B420;
constexpr u32 CH4_TADR = 0x1000B430;

constexpr u32 CH5_CHCR = 0xB000C000;
constexpr u32 CH5_MADR = 0xB000C010;
constexpr u32 CH5_QWC =  0xB000C020;

constexpr u32 CH6_CHCR = 0xB000C400;
constexpr u32 CH6_MADR = 0xB000C410;
constexpr u32 CH6_QWC =  0xB000C420;
constexpr u32 CH6_TADR = 0xB000C430;

constexpr u32 CH7_CHCR = 0x1000C800;
constexpr u32 CH7_MADR = 0x1000C810;
constexpr u32 CH7_QWC =  0x1000C820;

constexpr u32 CH8_CHCR = 0x1000D000;
constexpr u32 CH8_MADR = 0x1000D010;
constexpr u32 CH8_QWC =  0x1000D020;
constexpr u32 CH8_SADR = 0x1000D080;

constexpr u32 CH9_CHCR = 0x1000D400;
constexpr u32 CH9_MADR = 0x1000D410;
constexpr u32 CH9_QWC =  0x1000D420;
constexpr u32 CH9_TADR = 0x1000D430;
constexpr u32 CH9_SADR = 0x1000D480;

constexpr u32 VIF0_CHCR = CH0_CHCR;
constexpr u32 VIF0_MADR = CH0_MADR;
constexpr u32 VIF0_QWC = CH0_QWC;
constexpr u32 VIF0_TADR = CH0_TADR;
constexpr u32 VIF0_ASR0 = CH0_ASR0;
constexpr u32 VIF0_ASR1 = CH0_ASR1;

constexpr u32 VIF1_CHCR = CH1_CHCR;
constexpr u32 VIF1_MADR = CH1_MADR;
constexpr u32 VIF1_QWC = CH1_QWC;
constexpr u32 VIF1_TADR = CH1_TADR;
constexpr u32 VIF1_ASR0 = CH1_ASR0;
constexpr u32 VIF1_ASR1 = CH1_ASR1;

constexpr u32 GIF_CHCR = CH2_CHCR;
constexpr u32 GIF_MADR = CH2_MADR;
constexpr u32 GIF_QWC = CH2_QWC;
constexpr u32 GIF_TADR = CH2_TADR;
constexpr u32 GIF_ASR0 = CH2_ASR0;
constexpr u32 GIF_ASR1 = CH2_ASR1;

constexpr u32 IPU_FROM_CHCR = CH3_CHCR;
constexpr u32 IPU_FROM_MADR = CH3_MADR;
constexpr u32 IPU_FROM_QWC = CH3_QWC;

constexpr u32 IPU_TO_CHCR = CH4_CHCR;
constexpr u32 IPU_TO_MADR = CH4_MADR;
constexpr u32 IPU_TO_QWC = CH4_QWC;
constexpr u32 IPU_TO_TADR = CH4_TADR;

constexpr u32 SIF0_CHCR = CH5_CHCR;
constexpr u32 SIF0_MADR = CH5_MADR;
constexpr u32 SIF0_QWC = CH5_QWC;

constexpr u32 SIF1_CHCR = CH6_CHCR;
constexpr u32 SIF1_MADR = CH6_MADR;
constexpr u32 SIF1_QWC = CH6_QWC;
constexpr u32 SIF1_TADR = CH6_TADR;

constexpr u32 SIF2_CHCR = CH7_CHCR;
constexpr u32 SIF2_MADR = CH7_MADR;
constexpr u32 SIF2_QWC = CH7_QWC;

constexpr u32 SPR_FROM_CHCR = CH8_CHCR;
constexpr u32 SPR_FROM_MADR = CH8_MADR;
constexpr u32 SPR_FROM_QWC = CH8_QWC;
constexpr u32 SPR_FROM_SADR = CH8_SADR;

constexpr u32 SPR_TO_CHCR = CH9_CHCR;
constexpr u32 SPR_TO_MADR = CH9_MADR;
constexpr u32 SPR_TO_QWC = CH9_QWC;
constexpr u32 SPR_TO_TADR = CH9_TADR;
constexpr u32 SPR_TO_SADR = CH9_SADR;

// Common DMA Registers
constexpr u32 D_CTRL = 0x1000E000;
constexpr u32 D_STAT = 0x1000E010;
constexpr u32 D_PCR = 0x1000E020;
constexpr u32 D_SQWC = 0x1000E030;
constexpr u32 D_RBSR = 0x1000E040;
constexpr u32 D_RBOR = 0x1000E050;
constexpr u32 D_STADR = 0x1000E060;
constexpr u32 D_ENABLER = 0x1000F520;
constexpr u32 D_ENABLEW = 0x1000F590;

// FIFO Addresses (Probably fine to include here)

constexpr u32 VIF0_FIFO = 0x10004000;
constexpr u32 VIF1_FIFO = 0x10005000;
constexpr u32 GIF_FIFO = 0x10006000;
constexpr u32 IPU_FROM_FIFO = 0x10007000;
constexpr u32 IPU_TO_FIFO = 0x10007010;
