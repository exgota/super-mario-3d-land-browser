#ifndef OBSERVED_SKELETAL_ANIMATION_CONSTRUCTION_H
#define OBSERVED_SKELETAL_ANIMATION_CONSTRUCTION_H
#include <stddef.h>

// ARM32 views reconstructed from the EU executable. These are descriptive names,
// not recovered class identities. Unknown fields and allocation padding stay opaque.
namespace skeletal_construction {
struct Allocator;
struct AllocatorVtable {
    void* slots00[2];
    void* (*allocate)(Allocator*, unsigned, int);
    void (*release)(Allocator*, void*);
};
struct Allocator { AllocatorVtable* vtable; };
struct Evaluator;
struct EvaluatorVtable {
    void* slots00[3];
    void (*setModel)(Evaluator*, void*);
};
struct Evaluator { EvaluatorVtable* vtable; unsigned char opaque04[0x59]; unsigned char disableTranslation; };
struct WordVector { Allocator* allocator; unsigned* begin; unsigned* end; unsigned capacityAndFlags; };
struct Blend {
    EvaluatorVtable* vtable; Allocator* allocator; void* model; unsigned kind;
    WordVector children, weights, cachedWeights;
    unsigned char componentMode, dirty, normalize, opaque43;
};
struct PointerArray { int count, capacity; Evaluator** entries; };
struct BindingTable {
    unsigned char opaque00[0xc]; unsigned* begin; unsigned* end;
    unsigned char opaque14[8]; Evaluator** entries;
    unsigned char opaque20[8]; int stride;
};
struct Model {
    unsigned char opaque00[8]; unsigned char* resource;
    unsigned char opaque0c[0x1c]; BindingTable* bindings;
    unsigned char opaque2c[0x1f4]; void* heap;
    unsigned char opaque224[4]; void* context; int bindingIndex;
};
struct Archive { unsigned char opaque00[0x98]; unsigned char** resource; };
struct Player {
    const void* vtable; void* animationTable; unsigned char flags08[4];
    Model* model; Evaluator* single; Blend* blend; PointerArray evaluators;
    void* workspace;
};
struct EvaluationArguments { void* resource; int modelCount, maximumTrackCount; unsigned char useCache; };
static_assert(sizeof(Player)==0x28, "caller allocates 0x28 bytes");
static_assert(sizeof(Blend)==0x44, "allocator requests 0x44 bytes");
static_assert(sizeof(WordVector)==0x10, "four-word vector");
static_assert(offsetof(Model, heap)==0x220, "observed heap");
static_assert(offsetof(Model, context)==0x228, "observed model context");
static_assert(offsetof(Evaluator, disableTranslation)==0x5d, "translation mask");
static_assert(offsetof(Blend, componentMode)==0x40, "blend mode");
static_assert(sizeof(EvaluationArguments)==0x10, "creation arguments");
}
extern "C" skeletal_construction::Player* fn_001C440C(
    skeletal_construction::Player*, skeletal_construction::Archive*,
    skeletal_construction::Model*, skeletal_construction::Allocator*, int);
#endif
