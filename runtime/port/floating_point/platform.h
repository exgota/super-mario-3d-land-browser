#ifndef SUPER_MARIO_3D_LAND_SOFTFLOAT_PLATFORM_H
#define SUPER_MARIO_3D_LAND_SOFTFLOAT_PLATFORM_H

/* Use portable integer primitives. No host arithmetic or compiler intrinsics. */
#if !defined(__BYTE_ORDER__) || __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error "The checked SoftFloat platform requires little-endian storage"
#endif
#ifndef THREAD_LOCAL
#error "The checked SoftFloat platform requires C11 thread-local state"
#endif
#define LITTLEENDIAN 1
#define INLINE inline

#endif
