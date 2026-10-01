#pragma once

#include <math/seadMatrix.h>
#include <stddef.h>

// Existing map identities, declared without reconstructing unrelated methods.
namespace nn { namespace math { struct MTX34 { float m[3][4]; }; } }
namespace sead
{
template <typename T> class Matrix34CalcCtr
{
public:
    static void copy(nn::math::MTX34&, const nn::math::MTX34&);
};
class Random
{
public:
    unsigned int getU32();
};
}


extern "C" void fn_00291470(sead::Matrix34f&, const sead::Matrix34f&);

// Binary-derived partial views. These descriptive names do not claim the
// original effect library's class spelling. Opaque spans preserve observed
// offsets; no span was chosen to influence the compiler's instruction choices.
namespace effect_creation_detail
{
struct Context;
struct Set;
struct Emitter;
struct EmitterResource;

// Copy construction and assignment are independently different retail paths.
struct StoredMatrix : sead::Matrix34f
{
    StoredMatrix() {}
    StoredMatrix(const StoredMatrix& source) { fn_00291470(*this, source); }
    StoredMatrix& operator=(const StoredMatrix& source)
    {
        sead::Matrix34CalcCtr<float>::copy(
            reinterpret_cast<nn::math::MTX34&>(*this),
            reinterpret_cast<const nn::math::MTX34&>(source));
        return *this;
    }
};
struct StoredVector3 : sead::Vector3f
{
    StoredVector3& operator=(const StoredVector3& source)
    {
        x = source.x; y = source.y; z = source.z;
        return *this;
    }
};
struct StoredVector2 : sead::Vector2f
{
    StoredVector2& operator=(const StoredVector2& source)
    {
        x = source.x; y = source.y;
        return *this;
    }
};

struct RandomState
{
    unsigned short mValue0;
    unsigned short mValue2;
    unsigned int mState;
};

struct EmitterParameters
{
    unsigned int mValue0;
    unsigned int mValue4;
    unsigned int mValue8;
    unsigned int mValueC;
    unsigned int mValue10;
    unsigned int mValue14;
};

struct Set
{
    Context* mOwner;                                      // 0x000
    int mLiveEmitterCount;                                // 0x004
    int mEmitterCount;                                    // 0x008
    unsigned int mIdentifier;                             // 0x00c
    Emitter* mEmitters[8];                                 // 0x010
    EmitterParameters mEmitterParameters[8];              // 0x030
    int mBankIndex;                                       // 0x0f0
    int mResourceIndex;                                   // 0x0f4
    unsigned int mValueF8;                           // 0x0f8
    StoredMatrix mMatrixFC;                             // 0x0fc
    StoredMatrix mMatrix12C;                            // 0x12c
    StoredVector3 mVector15C;                             // 0x15c
    StoredVector2 mVector168;                             // 0x168
    StoredVector2 mVector170;                             // 0x170
    StoredVector2 mVector178;                             // 0x178
    StoredVector3 mVector180;                             // 0x180
    float mValues18C[4];                                  // 0x18c
    float mValue19C; float mValue1A0; float mValue1A4;                             // 0x19c
    StoredVector3 mVector1A8;                             // 0x1a8
    StoredVector3 mVector1B4;                        // 0x1b4
    unsigned int mValue1C0;                               // 0x1c0
    unsigned int mValue1C4;                               // 0x1c4
    unsigned int mValue1C8;                          // 0x1c8
    unsigned char mFlag1CC;
    unsigned char mFlag1CD;
    unsigned char mFlag1CE;
    unsigned char mFlag1CF;
};

struct Reference
{
    Set* mSet;
    unsigned int mIdentifier;
};

struct EmitterResource
{
    unsigned int mImplementationIndex;                    // 0x00
    unsigned char mOpaque04[4];
    unsigned int mRandomSeed;                             // 0x08
    unsigned char mOpaque0C[0x6c];
    int mValue78;                                         // 0x78
    int mRandomRange;                                     // 0x7c
    unsigned char mOpaque80[4];
    unsigned int mDeletionFilter;                         // 0x84
};

struct Emitter
{
    unsigned int mValue00;                                // 0x00
    int mValue04;                                         // 0x04
    unsigned int mGroup;                                  // 0x08
    Set* mSet;                                            // 0x0c
    EmitterParameters* mParameters;                       // 0x10
    unsigned int mIdentifier;                             // 0x14
    unsigned char mOpaque18[0x60];
    RandomState mRandom;                                  // 0x78
    int mValue80;                                         // 0x80
    float mValue84;                                       // 0x84
    int mValue88;                                         // 0x88
    unsigned int mValue8C;
    unsigned int mValue90;
    unsigned int mValue94;
    unsigned char mOpaque98[4];
    Emitter* mPrevious;                                   // 0x9c
    Emitter* mNext;                                       // 0xa0
    void* mImplementation;                                // 0xa4
    EmitterResource* mResource;                           // 0xa8
    void* mListAC;                                        // 0xac
    void* mListB0;                                        // 0xb0
};

struct ResourceMember
{
    unsigned char mOpaque00[4];
    EmitterResource* mResource;
};

struct ResourceSet
{
    unsigned char mOpaque00[4];
    ResourceMember* mMembers;                             // 0x04
    int mEmitterCount;                                    // 0x08
    unsigned char mOpaque0C[0x1c];
};

struct ResourceBank
{
    unsigned char mOpaque00[0x18];
    ResourceSet* mSets;
};

struct Context
{
    void* mHeap;                                          // 0x000
    ResourceBank** mBanks;                                // 0x004
    int mBankCount;                                       // 0x008
    void* mOpaque0C;
    Set* mSets;                                           // 0x010
    int mSetCapacity;                                     // 0x014
    unsigned int mSetMask;                                // 0x018
    Emitter* mGroupHeads[256];                            // 0x01c
    Emitter* mEmitters;                                   // 0x41c
    void* mOpaque420;
    void* mOpaque424;
    unsigned char mOpaque428[0x404];
    unsigned int mEmitterCursor;                          // 0x82c
    unsigned char mOpaque830[4];
    unsigned int mSetCursor;                              // 0x834
    unsigned char mOpaque838[4];
    int mEmitterCapacity;                                 // 0x83c
    unsigned char mOpaque840[8];
    unsigned int mEmitterMask;                            // 0x848
    unsigned char mOpaque84C[0x18];
    int mLiveSetCount;                                    // 0x864
    int mFreeEmitterCount;                                // 0x868
    unsigned char mOpaque86C[0x10];
    sead::Matrix34f mTemporaryMatrix;                      // 0x87c
    unsigned int mNextIdentifier;                         // 0x8ac
    unsigned char mOpaque8B0[0x10];
    void* mImplementations[3];                            // 0x8c0
};

// Assertions concern the 32-bit ARM layouts, not host pointer-width layouts.
#if defined(__arm__)
typedef char SetSizeCheck[sizeof(Set) == 0x1d0 ? 1 : -1];
typedef char EmitterSizeCheck[sizeof(Emitter) == 0xb4 ? 1 : -1];
typedef char ParametersSizeCheck[sizeof(EmitterParameters) == 0x18 ? 1 : -1];
typedef char ResourceSetSizeCheck[sizeof(ResourceSet) == 0x28 ? 1 : -1];
typedef char SetMatrixCheck[offsetof(Set, mMatrixFC) == 0xfc ? 1 : -1];
typedef char SetFlagsCheck[offsetof(Set, mFlag1CC) == 0x1cc ? 1 : -1];
typedef char EmitterRandomCheck[offsetof(Emitter, mRandom) == 0x78 ? 1 : -1];
typedef char ContextEmitterCheck[offsetof(Context, mEmitters) == 0x41c ? 1 : -1];
typedef char ContextCursorCheck[offsetof(Context, mEmitterCursor) == 0x82c ? 1 : -1];
typedef char ContextCapacityCheck[offsetof(Context, mFreeEmitterCount) == 0x868 ? 1 : -1];
typedef char ContextMatrixCheck[offsetof(Context, mTemporaryMatrix) == 0x87c ? 1 : -1];
typedef char ContextImplementationCheck[offsetof(Context, mImplementations) == 0x8c0 ? 1 : -1];
#endif
}

struct EffectGlobalRandomHolder
{
    sead::Random* mInstance;
    void* mDisposer;
};
extern "C" EffectGlobalRandomHolder dat_003E26DC;
extern "C" void fn_002200FC(effect_creation_detail::Context*, const char*);
