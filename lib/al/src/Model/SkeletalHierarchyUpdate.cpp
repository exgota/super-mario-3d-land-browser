// Original EU hierarchy dispatcher, 0x00338D80. Clean-room reconstruction.
// NonMatching: the project checker remains the sole byte-exact acceptance gate.
#include <stddef.h>

namespace hierarchy338d80 {
struct Vec3 { float x, y, z; };
struct Matrix34 { float v[12]; };
struct Record { Matrix34 matrix; Vec3 scale; unsigned flags; };
struct Buffer { unsigned count; Record* data; };
struct Matrices { unsigned count; Matrix34* begin; Matrix34* end; };
struct Controller;
struct CallbackArgs { Controller* controller; unsigned index; };
struct Callback { void (**vtable)(Callback*, CallbackArgs); };
struct Callbacks { unsigned unknown[2]; Callback** begin; Callback** end; };
struct Skeleton { unsigned unknown[7]; int dictionary; unsigned reserved; unsigned mode; unsigned flags; };
struct Joint { unsigned unknown; unsigned flags; unsigned identifier; int parent; };
struct Slots { void* unknown[3]; Buffer* (*local)(Controller*); void* unused10; Buffer* (*composed)(Controller*); void* unused18; Matrices* (*matrices)(Controller*); };
struct Controller { Slots* vtable; unsigned unknown; Skeleton* skeleton; unsigned char* owner; Callbacks* before; Callbacks* after; };
static_assert_(sizeof(Record) == 64);
static_assert_(offsetof(Controller, skeleton) == 8);
static_assert_(offsetof(Controller, after) == 0x14);
static_assert_(offsetof(Skeleton, dictionary) == 0x1c);
static_assert_(offsetof(Skeleton, mode) == 0x24);
static_assert_(offsetof(Joint, parent) == 0xc);
static_assert_(offsetof(Slots, local) == 0xc);
static_assert_(offsetof(Slots, composed) == 0x14);
static_assert_(offsetof(Slots, matrices) == 0x1c);
}
extern "C" {
hierarchy338d80::Record* fn_002164F8();
void fn_0033AA94(void*, hierarchy338d80::Record*, hierarchy338d80::Vec3*, const hierarchy338d80::Record*, const hierarchy338d80::Record*, const hierarchy338d80::Record*);
void fn_0025C3E4(void*, hierarchy338d80::Record*, hierarchy338d80::Vec3*, const hierarchy338d80::Record*, const hierarchy338d80::Record*, const hierarchy338d80::Record*);
void fn_0033A90C(void*, hierarchy338d80::Record*, hierarchy338d80::Vec3*, const hierarchy338d80::Record*, const hierarchy338d80::Record*, const hierarchy338d80::Record*);
void fn_002723E0(hierarchy338d80::Matrix34*, const hierarchy338d80::Record*, const hierarchy338d80::Vec3*);
}
namespace hierarchy338d80 {
inline bool has(unsigned value, unsigned mask) { return (value & mask) == mask; }
inline void callbacks(Callbacks* list, Controller* controller, unsigned index) {
    CallbackArgs args = {controller, index};
    Callback** end = list->end;
    for (Callback** it = list->begin; it != end; ++it)
        (*it)->vtable[0](*it, args);
}
inline Joint* joint(Skeleton* skeleton, unsigned index) {
    char* dictionary = skeleton->dictionary ? reinterpret_cast<char*>(&skeleton->dictionary) + skeleton->dictionary : 0;
    if (!dictionary) return 0;
    int* offset = reinterpret_cast<int*>(dictionary + 0x28 + 16 * index);
    return *offset ? reinterpret_cast<Joint*>(reinterpret_cast<char*>(offset) + *offset) : 0;
}
inline int bits(float x) { union { float f; int i; } u; u.f = x; return u.i; }
inline void clampScale(Vec3& scale) {
    float length = scale.x*scale.x + scale.y*scale.y + scale.z*scale.z;
    if (bits(length) < 0x358637be) {
        scale.x = scale.x < 0.0f ? -1.0000001111620804e-06f : 1.0000001111620804e-06f;
        scale.y = scale.y < 0.0f ? -1.0000001111620804e-06f : 1.0000001111620804e-06f;
        scale.z = scale.z < 0.0f ? -1.0000001111620804e-06f : 1.0000001111620804e-06f;
    }
}
inline void flags(Record* record) {
    record->flags &= ~0x7e0u;
    if (!has(record->flags, 8)) {
        record->flags &= ~0x600u;
        if (record->scale.x == record->scale.y && record->scale.x == record->scale.z) {
            record->flags |= 0x400;
            if (bits(record->scale.x) == 0x3f800000) record->flags |= 0x200;
        }
    }
}

}
#ifdef NON_MATCHING
extern "C" void fn_00338D80(void*, hierarchy338d80::Controller* controller, void* context) {
    using namespace hierarchy338d80;
    Skeleton* skeleton = controller->skeleton;
    bool rootIdentity = has(skeleton->flags, 1);
    Buffer* local = controller->vtable->local(controller);
    Buffer* composed = controller->vtable->composed(controller);
    Matrices* matrices = controller->vtable->matrices(controller);
    Matrix34* output = matrices->begin;
    Matrix34* end = matrices->end;
    unsigned mode = skeleton->mode & 0xff;
    if (mode == 1) {
    unsigned index = 0;
    for (; output != end; ++output, ++index) {
        callbacks(controller->before, controller, index);
        if (has(local->data[index].flags, 1)) {
            Joint* resource = joint(skeleton, index);
            const Record* parentLocal;
            const Record* parentComposed;
            if (resource->parent != -1) {
                parentLocal = local->data + resource->parent;
                parentComposed = composed->data + resource->parent;
            } else if (!rootIdentity) {
                parentComposed = reinterpret_cast<Record*>(controller->owner + 0xbc);
                parentLocal = reinterpret_cast<Record*>(controller->owner + 0x4c);
            } else {
                parentComposed = fn_002164F8();
                parentLocal = fn_002164F8();
            }
            Record* result = composed->data + index;
            Record* input = local->data + index;
            Vec3 scale;
            if (1 == 1 && (resource->flags & 0x20))
                fn_0033AA94(context, result, &scale, input, parentComposed, parentLocal);
            else if (1 == 2)
                fn_0033A90C(context, result, &scale, input, parentComposed, parentLocal);
            else
                fn_0025C3E4(context, result, &scale, input, parentComposed, parentLocal);
            clampScale(scale);
            result->scale = scale;
            fn_002723E0(output, result, 1 == 2 ? &result->scale : &input->scale);
            flags(result);
        }
        callbacks(controller->after, controller, index);
    }
    return;
    }
    if (mode == 0) {
    unsigned index = 0;
    for (; output != end; ++output, ++index) {
        callbacks(controller->before, controller, index);
        if (has(local->data[index].flags, 1)) {
            Joint* resource = joint(skeleton, index);
            const Record* parentLocal;
            const Record* parentComposed;
            if (resource->parent != -1) {
                parentLocal = local->data + resource->parent;
                parentComposed = composed->data + resource->parent;
            } else if (!rootIdentity) {
                parentComposed = reinterpret_cast<Record*>(controller->owner + 0xbc);
                parentLocal = reinterpret_cast<Record*>(controller->owner + 0x4c);
            } else {
                parentComposed = fn_002164F8();
                parentLocal = fn_002164F8();
            }
            Record* result = composed->data + index;
            Record* input = local->data + index;
            Vec3 scale;
            if (0 == 1 && (resource->flags & 0x20))
                fn_0033AA94(context, result, &scale, input, parentComposed, parentLocal);
            else if (0 == 2)
                fn_0033A90C(context, result, &scale, input, parentComposed, parentLocal);
            else
                fn_0025C3E4(context, result, &scale, input, parentComposed, parentLocal);
            clampScale(scale);
            result->scale = scale;
            fn_002723E0(output, result, 0 == 2 ? &result->scale : &input->scale);
            flags(result);
        }
        callbacks(controller->after, controller, index);
    }
    return;
    }
    {
    unsigned index = 0;
    for (; output != end; ++output, ++index) {
        callbacks(controller->before, controller, index);
        if (has(local->data[index].flags, 1)) {
            Joint* resource = joint(skeleton, index);
            const Record* parentLocal;
            const Record* parentComposed;
            if (resource->parent != -1) {
                parentLocal = local->data + resource->parent;
                parentComposed = composed->data + resource->parent;
            } else if (!rootIdentity) {
                parentComposed = reinterpret_cast<Record*>(controller->owner + 0xbc);
                parentLocal = reinterpret_cast<Record*>(controller->owner + 0x4c);
            } else {
                parentComposed = fn_002164F8();
                parentLocal = fn_002164F8();
            }
            Record* result = composed->data + index;
            Record* input = local->data + index;
            Vec3 scale;
            if (2 == 1 && (resource->flags & 0x20))
                fn_0033AA94(context, result, &scale, input, parentComposed, parentLocal);
            else if (2 == 2)
                fn_0033A90C(context, result, &scale, input, parentComposed, parentLocal);
            else
                fn_0025C3E4(context, result, &scale, input, parentComposed, parentLocal);
            clampScale(scale);
            result->scale = scale;
            fn_002723E0(output, result, 2 == 2 ? &result->scale : &input->scale);
            flags(result);
        }
        callbacks(controller->after, controller, index);
    }
    }
}

#endif
