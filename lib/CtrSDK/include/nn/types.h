// Basic integer and floating point types used across the game and its libraries.
// Written for this project from the ARM EABI type sizes; not derived from any SDK material.
#pragma once

#include <stddef.h>

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;
typedef signed char        s8;
typedef signed short       s16;
typedef signed int         s32;
typedef signed long long   s64;

typedef volatile u8  vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
typedef volatile s8  vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

typedef float  f32;
typedef double f64;

typedef u8  bit8;
typedef u16 bit16;
typedef u32 bit32;
typedef u64 bit64;

typedef u32 uptr;
typedef s32 sptr;
