#pragma once

#include <prim/seadSafeString.h>
#include <stddef.h>

// Private views reconstructed from the EU executable, not original class names.
// These math declarations are token-compatible with the existing copy source
// and the separately proposed emitter-child-update header.
namespace nn {
namespace math {
struct VEC3 { float x; float y; float z; };
struct MTX34 { float m[ 3 ][ 4 ]; };
}
}

extern "C" {
void _ZN4sead15Matrix34CalcCtrIfE4copyERN2nn4math5MTX34ERKS4_(nn::math::MTX34&, const nn::math::MTX34&);
float _ZN4sead14Vector3CalcCtrIfE9normalizeERN2nn4math4VEC3E(nn::math::VEC3&);
extern const nn::math::MTX34 dat_00430A88;
extern const float dat_003A2118;
extern const float dat_003A211C;
extern const unsigned _ZTVN4sead14SafeStringBaseIcEE[];

// Preserve the formatter's established variadic BufferedSafeString interface.
s32 fn_0028E1E4(sead::BufferedSafeString* receiver, const char* format, ...);
void fn_0021F264(void*);
void fn_0021F21C(void*);
void* __aeabi_vec_ctor_nocookie_nodtor(void*, void (*)(void*), unsigned, unsigned);
s32 fn_002499F4(sead::BufferedSafeString*, const sead::SafeString&, s32);
}

namespace editor2edfe8 {
typedef unsigned char Byte;

// Constructors 0021F264 and 0021F21C establish a BufferedSafeString prefix,
// followed by a 64- or 96-byte inline buffer. The precise derived type is open.
template <unsigned Capacity> struct StringStorage {
    const void* vtable;
    char* data;
    int capacity;
    char buffer[Capacity];
    sead::BufferedSafeString* string() {
        return reinterpret_cast<sead::BufferedSafeString*>(this);
    }
    const char* cstr() const {
        return reinterpret_cast<const sead::BufferedSafeString*>(this)->cstr();
    }
};
typedef StringStorage<64> String64;
typedef StringStorage<96> String96;
// Existing whole map row 003D9C34 supplies the SafeString address point at +8.
// This borrowed prefix does not introduce another compiled virtual table owner.
struct StringView {
    const void* addressPoint;
    const char* data;
    const sead::SafeString& string() const {
        return *reinterpret_cast<const sead::SafeString*>(this);
    }
};
struct Color { float r, g, b, a; };
struct Defaults {
    nn::math::VEC3 direction;                       // +00
    Color color0, color1, color2, color3;            // +0C..+3C
    nn::math::VEC3 scale;                           // +4C
    Color color4, color5;                           // +58, +68
    float scalar78;
    Color color6;                                  // +7C
    float scalar8c, scalar90, scalar94;
    unsigned word98;
};
struct Parameters {
    void* owner;                                   // +000
    nn::math::MTX34 matrix;                         // +004
    unsigned word034;
    float limit0, limit1, limit2, limit3;             // +038
    int integerLimit;                              // +048
    String64 range0, positive0, range2, positive2, smallPositive2;
    String96 autoPositive2, autoRange1, autoPositive1, autoRange3, autoPositive3;
    String64 integer0, integer1, disabledRange0, disabledPositive0, disabledPositive2;
    String96 disabledAutoRange1, disabledAutoPositive1;
    String64 disabledAutoRange3;
    String96 disabledAutoPositive3;
    String64 disabledInteger0, disabledInteger1;
    String64 largeInteger0, largeInteger1, disabledLargeInteger0, disabledLargeInteger1;
    Defaults defaults;                             // +8B8
    Byte unknown954[0x30];
    String64 indexed[16];                          // +984
    String64 numeric[2];                           // +E44
    Byte unknownEdc[4];
    float scalarEe0;
};
static_assert_(sizeof(String64) == 0x4C);
static_assert_(sizeof(String96) == 0x6C);
static_assert_(sizeof(StringView) == 8);
static_assert_(sizeof(Color) == 0x10);
static_assert_(sizeof(Defaults) == 0x9C);
static_assert_(offsetof(Parameters, range0) == 0x4C);
static_assert_(offsetof(Parameters, autoPositive2) == 0x1C8);
static_assert_(offsetof(Parameters, integer0) == 0x3E4);
static_assert_(offsetof(Parameters, disabledAutoRange3) == 0x638);
static_assert_(offsetof(Parameters, defaults) == 0x8B8);
static_assert_(offsetof(Parameters, indexed) == 0x984);
static_assert_(offsetof(Parameters, numeric) == 0xE44);
static_assert_(offsetof(Parameters, scalarEe0) == 0xEE0);
}

extern "C" void fn_00292D20(editor2edfe8::Color*);
extern "C" editor2edfe8::Parameters* fn_002EDFE8(editor2edfe8::Parameters*, void*);
