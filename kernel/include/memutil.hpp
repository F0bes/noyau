// SPDX-License-Identifier: MPL-2.0

#pragma once

#include <type_traits>

template<class T>
constexpr T _load(uintptr_t addr) { return *reinterpret_cast<volatile T*>(addr); }

constexpr u8 _l8(u32 addr) { return _load<u8>(addr); }
constexpr u16 _l16(u32 addr) { return _load<u16>(addr); }
constexpr u32 _l32(u32 addr) { return _load<u32>(addr); }
constexpr u64 _l64(u32 addr) { return _load<u64>(addr); }
constexpr u128 _l128(u32 addr) { return _load<u128>(addr); }

template<class T>
constexpr void _store(uintptr_t addr, T val)
{
	static_assert(std::is_trivial<T>::value);
	*reinterpret_cast<volatile T*>(addr) = val;
}

constexpr void _s8(u32 addr, u8 val) { _store(addr, val); }
constexpr void _s16(u32 addr, u16 val) { _store(addr, val); }
constexpr void _s32(u32 addr, u32 val) { _store(addr, val); }
constexpr void _s64(u32 addr, u64 val) { _store(addr, val); }
constexpr void _s128(u32 addr, u128 val) { _store(addr, val); }

// Ensure the previous loads or stores are completed
inline void _syncl() { asm volatile("sync.l\nnop\n"); }

// Ensure the previous instruction is completed
// Does not include mul,div, > 1 cycle COP1 and COP2 instructions
inline void _syncp() { asm volatile("sync.p\nnop\n"); }

template<bool Cached, bool Mapped>
constexpr u32 _seg(ee_ptr_t addr)
{
	addr = addr & 0x1FFFFFFF;

	if(Cached && Mapped)
		return 0xC0000000 | addr; // kseg2 Unsure if kseg2 is default cached or not. Assume it is
	else if (!Cached && Mapped)
		return 0x20000000 | addr; // Weird combination. Assume useg uncached??
	else if(Cached && !Mapped)
		return 0x80000000 | addr; // kseg0 (Can be uncached depending on K0 in COP0.config)
	else if(!Cached && !Mapped)
		return 0xA0000000 | addr; // kseg1
}

// Unmapped, cached
constexpr u32 _kseg0(ee_ptr_t addr) { return (addr & 0x1FFFFFFF) | 0x80000000; }
// Unmapped, uncached
constexpr u32 _kseg1(ee_ptr_t addr) { return (addr & 0x1FFFFFFF) | 0xA0000000; }
// Mapped
constexpr u32 _ksseg(ee_ptr_t addr) { return (addr & 0x1FFFFFFF) | 0xC0000000; }
// Mapped
constexpr u32 _kseg3(ee_ptr_t addr) { return (addr & 0x1FFFFFFF) | 0xE0000000; }

// Use for the DMAC
constexpr u32 _phys(ee_ptr_t addr) { return addr & 0x1FFFFFFF; }
