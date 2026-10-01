#ifndef CLEAN_TRANSFORM_BLEND_ROOT_H
#define CLEAN_TRANSFORM_BLEND_ROOT_H
#include <stddef.h>

// Retail-observed ARM32 views, not asserted original class names or full sizes.
namespace transform_blend {
struct Matrix34 { float m[3][4]; };
struct Transform { Matrix34 matrix; float scale[3]; unsigned flags; };
struct TypeNode { TypeNode* parent; };
struct Evaluator;
struct EvaluatorVtable {
    void* slot00; void* slot04;
    TypeNode* (*type)(Evaluator*);
    void* slot0c; void* slot10; void* slot14;
    Transform* (*evaluate)(Evaluator*, Transform*, int);
    int (*hasIndex)(Evaluator*, int);
};
struct Evaluator { EvaluatorVtable* vtable; unsigned char opaque[0x57]; unsigned char disabled[3]; };
struct BlendPolicy;
struct BlendPolicyVtable {
    void* slot00; void* slot04;
    int (*blend)(BlendPolicy*, Transform*, unsigned, Transform*, const float*);
    int (*finish)(BlendPolicy*, Transform*, unsigned);
};
struct BlendPolicy { BlendPolicyVtable* vtable; unsigned char opaque04; unsigned char needsFinish; };
struct BindPose { float scale[3]; float rotation[3]; float translation[3]; };
struct Model {
    unsigned char opaque00[8]; unsigned char* resource;
    unsigned opaque0c; BlendPolicy** policies;
    unsigned char opaque14[0x40]; BindPose** poses;
};
struct Root {
    unsigned char opaque00[8]; Model* model;
    unsigned char opaque0c[8]; Evaluator** children; Evaluator** childrenEnd;
    unsigned char opaque1c[8]; float* weights; float* weightsEnd;
    unsigned char opaque2c[8]; float* cachedWeights;
    unsigned char opaque38[8]; unsigned char componentMode, dirty, normalize;
};
static_assert(sizeof(Matrix34)==0x30, "matrix extent");
static_assert(sizeof(Transform)==0x40, "transform extent");
// Constructor 001C440C requests 0x44 bytes and installs vtable 003D8500.
static_assert(sizeof(Root)==0x44, "observed root allocation");
static_assert(offsetof(Transform, flags)==0x3c, "transform flags");
static_assert(offsetof(Evaluator, disabled)==0x5b, "channel masks");
static_assert(offsetof(Model, poses)==0x54, "bind poses");
static_assert(offsetof(Root, componentMode)==0x40, "blend mode");
}
extern "C" transform_blend::Transform* fn_0033BA3C(transform_blend::Root*, transform_blend::Transform*, int);
#endif
