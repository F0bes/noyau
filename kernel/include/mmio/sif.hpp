// SPDX-License-Identifier: MPL-2.0

#pragma once

// Only writable from the EE
constexpr u32 SIF_MSCOM = 0x1000F200;
// Only writable from the IOP
constexpr u32 SIF_SMCOM = 0x1000F210;
// Writable from the EE. The IOP can read and clear bits.
constexpr u32 SIF_MSFLG = 0x1000F220;
// Writable from the IOP. The EE can read and clear bits.
constexpr u32 SIF_SMFLG = 0x1000F230;
constexpr u32 SIF_CTRL = 0x1000F240;
constexpr u32 SIF_F260 = 0x1000F260;
