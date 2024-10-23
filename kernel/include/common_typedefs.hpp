// SPDX-License-Identifier: MPL-2.0

#pragma once

#include <stddef.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uint32_t u128 __attribute__((mode(TI)));

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef int32_t s128 __attribute__((mode(TI)));

typedef union {
	u8  b[16];
	u16 h[8];
	u32 w[4];
	u64 d[2];
	u128 q[1];
} qword_t __attribute__((aligned(16)));

#ifdef __cplusplus

struct ee_ptr_t
{
	u32 address;

	constexpr ee_ptr_t() : address(0) {}
	constexpr ee_ptr_t(u32 addr) : address(addr) {}
	constexpr ee_ptr_t(void* ptr) : address(reinterpret_cast<u32>(ptr)) {}

	constexpr operator u32() const { return address; }
};

typedef ee_ptr_t iop_ptr_t;

#endif

#define STRUCT_PACKED __attribute__((packed))
#define STRUCT_ALIGNED __attribute__((aligned(16)))
#define STRUCT_ALIGNED_PACKED __attribute__((packed, aligned(16)))
