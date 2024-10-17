// SPDX-License-Identifier: MPL-2.0

#pragma once

constexpr u32 T0_COUNT = 0x10000000;
constexpr u32 T0_MODE = T0_COUNT + 0x10;
constexpr u32 T0_COMPARE = T0_COUNT + 0x20;
constexpr u32 T0_HOLD = T0_COUNT + 0x30;

constexpr u32 T1_COUNT = 0x10000800;
constexpr u32 T1_MODE = T1_COUNT + 0x10;
constexpr u32 T1_COMPARE = T1_COUNT + 0x20;
constexpr u32 T1_HOLD = T1_COUNT + 0x30;

constexpr u32 T2_COUNT = 0x10001000;
constexpr u32 T2_MODE = T2_COUNT + 0x10;
constexpr u32 T2_COMPARE = T2_COUNT + 0x20;

constexpr u32 T3_COUNT = 0x10001800;
constexpr u32 T3_MODE = T3_COUNT + 0x10;
constexpr u32 T3_COMPARE = T3_COUNT + 0x20;
