#pragma once

// Private views recovered from EU 002ECBD0 and its direct callers/callees.
// Names describe observed roles; they do not assert original library identities.
namespace nn {
namespace math {
struct VEC3 { float x; float y; float z; };
struct MTX34 { float m[ 3 ][ 4 ]; };
}
}

extern "C" {
void _ZN4sead15Matrix34CalcCtrIfE4copyERN2nn4math5MTX34ERKS4_(nn::math::MTX34&, const nn::math::MTX34&);
void _ZN4sead14Vector3CalcCtrIfE3addERN2nn4math4VEC3ERKS4_S7_(nn::math::VEC3&, const nn::math::VEC3&, const nn::math::VEC3&);
void _ZN4sead14Vector3CalcCtrIfE3subERN2nn4math4VEC3ERKS4_S7_(nn::math::VEC3&, const nn::math::VEC3&, const nn::math::VEC3&);
void _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(nn::math::VEC3&, const nn::math::VEC3&, float);
float _ZN4sead14Vector3CalcCtrIfE9normalizeERN2nn4math4VEC3E(nn::math::VEC3&);
void _ZN4sead14Vector3CalcCtrIfE5crossERN2nn4math4VEC3ERKS4_S7_(nn::math::VEC3&, const nn::math::VEC3&, const nn::math::VEC3&);
extern const nn::math::MTX34 dat_00430A88;
extern void* dat_003EF914;
extern const nn::math::VEC3* dat_003EFA14;
extern const nn::math::VEC3* dat_003EFA18;
}

namespace emitter2ecbd0 {
typedef unsigned char Byte;
struct Vec3 : nn::math::VEC3 {
    void set(const nn::math::VEC3& a) { x = a.x; y = a.y; z = a.z; }
    Vec3 operator-(const Vec3& b) const {
        Vec3 out;
        _ZN4sead14Vector3CalcCtrIfE3subERN2nn4math4VEC3ERKS4_S7_(out, *this, b);
        return out;
    }
    Vec3 operator*(float scalar) const {
        Vec3 out;
        _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(out, *this, scalar);
        return out;
    }
    void add(const Vec3& b) { _ZN4sead14Vector3CalcCtrIfE3addERN2nn4math4VEC3ERKS4_S7_(*this, *this, b); }
    void normalize() { _ZN4sead14Vector3CalcCtrIfE9normalizeERN2nn4math4VEC3E(*this); }
};
struct Vec2 { float x, y; };
struct Vec4 { float x, y, z, w; };
struct Word3 { unsigned x, y, z; };
struct Matrix34 : nn::math::MTX34 {
    void copy(const nn::math::MTX34& m) { _ZN4sead15Matrix34CalcCtrIfE4copyERN2nn4math5MTX34ERKS4_(*this, m); }
};
struct TrailConfig { int orientation, capacity; Byte unknown08[0x14]; float smoothing; };
struct ChildConfig {
    int count, startPercent, life, spacing;                 // +00
    float velocityScale, randomVelocity, randomX, randomY;  // +10
    float randomZ, randomPosition;                         // +20
    Byte unknown028[0x24];
    Vec4 color;                                           // +4C
    Byte unknown05C[0xC];
    float alpha, endAlpha, startAlpha, inheritScale;         // +68
    Vec2 scale;                                           // +78
    Byte unknown080[4];
    Word3 angles, angleRange, angularVelocity, angularRange;// +84
    Byte unknown0B4[0x14];
    int alphaEndTime, alphaStartTime, scaleEndTime;          // +C8
    Vec2 endScale, textureStep;                            // +D4
};
struct Resource {
    Byte unknown000[0x2D];
    Byte useEmitterMatrices;
    Byte unknown02E[0x66];
    int shape;                                            // +94
    Byte unknown098[0x15C];
    unsigned short childFlags;                            // +1F4
    Byte unknown1F6[4];
    unsigned short trailFlags;                            // +1FA
    Byte unknown1FC[6];
    unsigned short trailOffset;                           // +202
    Byte unknown204[4];
    ChildConfig child;                                    // +208
};
struct Owner {
    Byte unknown000[0x178];
    float sizeX;
    Byte unknown17C[0x10];
    Vec3 color;                                           // +18C
    float alpha;                                          // +198
};
struct TrailSample { Vec3 position; float width; Matrix34 matrix; Vec3 direction; };
struct Trail {
    unsigned slot;
    int begin, end;
    TrailSample samples[128];                             // +C, stride 4C
    int count;                                            // +260C
    Byte unknown2610[0xC];
    Vec3 color; float alpha;                               // +261C
    Vec4 texture;                                         // +262C
    Byte unknown263C[8];
    Matrix34 matrix;                                      // +2644
    Vec3 direction;                                       // +2674
};
struct Particle {
    Vec3 position, localPosition, velocity;                // +00
    Word3 angles, angularVelocity;                        // +24
    Matrix34 rotation, matrix;                            // +3C
    float alpha, alphaBeginStep, alphaEndStep;              // +9C
    Byte unknown0A8[8];
    Vec2 scale, scaleStep;                                 // +B0
    Byte unknown0C0[0x10];
    Vec4 texture;                                         // +D0
    Vec4 color;                                           // +E0
    int frame, life;
    unsigned seed;
    int childTimer;                                       // +FC
    float alphaScale, sizeScale;
    Vec3 emissionVelocity;                                // +108
    Resource* resource;
    Particle* next;
    Particle* previous;
    Trail* trail;                                         // +120
};
struct State {
    Byte unknown000[0xC];
    Owner* owner;
    Byte unknown010[8];
    Matrix34 rotation, matrix;                            // +18, +48
    unsigned short randomIndexA, randomIndexB;             // +78, +7A
    unsigned random;                                      // +7C
    Byte unknown080[4];
    float fade;
    Byte unknown088[0x20];
    Resource* resource;                                   // +A8
    Particle* particles;
    Particle* children;                                   // +B0
};
static_assert_(sizeof(Vec3) == 12);
static_assert_(sizeof(Matrix34) == 48);
static_assert_(sizeof(TrailSample) == 0x4C);
static_assert_(sizeof(ChildConfig) == 0xE4);
static_assert_(sizeof(Particle) == 0x124);
static_assert_(sizeof(Resource) == 0x2EC);
static_assert_(sizeof(Trail) == 0x2680);
inline unsigned randomByte(State* s) {
    unsigned value = s->random;
    s->random = value * 0x41C64E6DU + 12345U;
    return value >> 24;
}
}
extern "C" emitter2ecbd0::Trail* fn_002E5984(void*, emitter2ecbd0::State*, emitter2ecbd0::Particle*);
extern "C" emitter2ecbd0::Particle* fn_0021FFCC(void*);
extern "C" void fn_002ECBD0(void*, emitter2ecbd0::State*, emitter2ecbd0::Particle*);
