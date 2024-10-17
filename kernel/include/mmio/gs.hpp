// SPDX-License-Identifier: MPL-2.0

#pragma once

// clang-format off

// GIF common
constexpr u32 GIF_CTRL  = 0x10003000;
constexpr u32 GIF_MODE  = 0x10003010;
constexpr u32 GIF_STAT  = 0x10003020;
constexpr u32 GIF_TAG0  = 0x10003040;
constexpr u32 GIF_TAG1  = 0x10003050;
constexpr u32 GIF_TAG2  = 0x10003060;
constexpr u32 GIF_TAG3  = 0x10003070;
constexpr u32 GIF_CNT   = 0x10003080;
constexpr u32 GIF_P3CNT = 0x10003090;
constexpr u32 GIF_P3TAG = 0x100030A0;

// GS Privileged
// "...must be accessed using LD/SD instructions."
// Can we guarantee this with our memory macros?

constexpr u32 GS_PMODE    = 0x12000000;
constexpr u32 GS_SMODE1   = 0x12000010;
constexpr u32 GS_SMODE2   = 0x12000020;
constexpr u32 GS_SRFSH    = 0x12000030;
constexpr u32 GS_SYNCH1   = 0x12000040;
constexpr u32 GS_SYNCH2   = 0x12000050;
constexpr u32 GS_SYNCV    = 0x12000060;
constexpr u32 GS_DISPFB1  = 0x12000070;
constexpr u32 GS_DISPLAY1 = 0x12000080;
constexpr u32 GS_DISPFB2  = 0x12000090;
constexpr u32 GS_DISPLAY2 = 0x120000A0;
constexpr u32 GS_EXTBUF   = 0x120000B0;
constexpr u32 GS_EXTDATA  = 0x120000C0;
constexpr u32 GS_EXTWRITE = 0x120000D0;
constexpr u32 GS_BGCOLOR  = 0x120000E0;
constexpr u32 GS_CSR      = 0x12001000;
constexpr u32 GS_IMR      = 0x12001010;
constexpr u32 GS_BUSDIR   = 0x12001040;
constexpr u32 GS_SIGLBLID = 0x12001080;

// clang-format on
