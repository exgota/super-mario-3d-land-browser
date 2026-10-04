#pragma once
#include <nn/hid/CTR/hid_HidBase.h>
#include <nn/math/math_MatrixStorage.h>
#include <nn/math/math_Vector3.h>
namespace observed_hid {
struct Sample { s16 axis[3]; };
struct Reader {
    nn::hid::CTR::HidBase* device;
    s16 threshold;
    s16 gain;
    Sample previous;
    Sample bias;
    u32 unknown14;
    nn::math::MTX34 transform;
    bool biasEnabled;
    bool transformEnabled;
    u8 unknown4A[2];
    s32 index;
    s64 timestamp;
};
static_assert(sizeof(nn::hid::CTR::HidBase) == 8, "reference and binary device layout");
static_assert(sizeof(Sample) == 6, "three signed16 axes");
static_assert(offsetof(Reader, previous) == 8, "previous sample");
static_assert(offsetof(Reader, bias) == 0xe, "three biases");
static_assert(offsetof(Reader, transform) == 0x18, "three-by-four transform");
static_assert(offsetof(Reader, biasEnabled) == 0x48, "bias flag");
static_assert(offsetof(Reader, transformEnabled) == 0x49, "transform flag");
static_assert(offsetof(Reader, index) == 0x4c, "ring index");
static_assert(offsetof(Reader, timestamp) == 0x50, "signed64 timestamp");
}
