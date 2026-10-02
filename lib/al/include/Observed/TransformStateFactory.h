#ifndef OBSERVED_TRANSFORM_STATE_FACTORY_H
#define OBSERVED_TRANSFORM_STATE_FACTORY_H
#include <stddef.h>

// These two definitions are token-identical to the clean 0033BA3C proposal.
// They describe consumed ARM32 storage, not recovered original class identities.
namespace transform_blend {
struct Matrix34 { float m[3][4]; };
struct Transform { Matrix34 matrix; float scale[3]; unsigned flags; };
}
namespace skeletal_construction {
struct Allocator;
struct AllocatorVtable {
    void* slots00[2];
    void* (*allocate)(Allocator*, unsigned, int);
    void (*release)(Allocator*, void*);
};
struct Allocator { AllocatorVtable* vtable; };
}
namespace transform_state {
using transform_blend::Matrix34;
using transform_blend::Transform;
using skeletal_construction::Allocator;
struct Resource { unsigned char opaque00[0x18]; int count; };
template<class T> struct Array {
    Allocator* allocator; T* begin; T* end; unsigned capacityAndFlags;
};
struct Root {
    const unsigned* vtable; Allocator* allocator; Resource* resource;
    unsigned opaque0c; void* storage10; void* storage14;
    unsigned char flag18, opaque19[3];
    Array<Transform> supplied, calculated;
    Array<Matrix34> matrices, auxiliaryMatrices;
    Resource* source;
};
static_assert(sizeof(Matrix34)==0x30, "observed matrix copy span");
static_assert(sizeof(Transform)==0x40, "observed transform stride");
static_assert(sizeof(Array<Transform>)==0x10, "four-word ownership record");
static_assert(offsetof(Root,supplied)==0x1c, "supplied array offset");
static_assert(offsetof(Root,calculated)==0x2c, "calculated array offset");
static_assert(offsetof(Root,matrices)==0x3c, "matrix array offset");
static_assert(offsetof(Root,auxiliaryMatrices)==0x4c, "auxiliary matrix offset");
static_assert(sizeof(Root)==0x60, "allocator receiver request");
}
extern "C" transform_state::Root* fn_002A2C9C(transform_state::Resource*, unsigned,
    signed char, transform_state::Array<transform_blend::Transform>*,
    skeletal_construction::Allocator*);
#endif
