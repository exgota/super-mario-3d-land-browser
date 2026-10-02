#include <Observed/SkeletalAnimationConstruction.h>

using namespace skeletal_construction;
extern "C" {
Player* fn_0024E47C(Player*);
void* fn_001D956C(void*, Allocator*, int, void*);
void* fn_0024E598(void*, int);
void fn_0024E568(void*, const char*, void*);
void* fn_0024E49C(Allocator*, const char*);
void* fn_00293088(void*);
void fn_0026AC60(PointerArray*, int, void*, int);
Evaluator* fn_002A63D8(EvaluationArguments*, Allocator*);
extern const unsigned dat_003D62A0[];
extern const unsigned dat_003D8008[];
extern const unsigned dat_003D82A4[];
extern const unsigned dat_003D8500[];
extern void* dat_003E23B8;
}
#ifdef NON_MATCHING
namespace {
struct Workspace { unsigned opaque[6]; };
struct AnimationTable { unsigned opaque[2]; };
unsigned char* relative(unsigned char* field) {
    int offset = *reinterpret_cast<int*>(field);
    return offset ? field + offset : 0;
}
unsigned char* skeletonResource(Model* model) {
    unsigned char* resource = model->resource;
    if (resource && ((*reinterpret_cast<unsigned*>(resource) & 0x40000092u) != 0x40000092u))
        resource = 0;
    return relative(resource + 0xe0);
}
void resetVector(WordVector& vector) {
    vector.allocator = 0;
    vector.begin = 0;
    vector.end = 0;
    vector.capacityAndFlags = 0;
}
void replaceVector(WordVector& vector, Allocator* allocator, unsigned* storage, int capacity) {
    WordVector temporary;
    temporary.allocator = allocator;
    temporary.begin = storage;
    temporary.end = storage;
    temporary.capacityAndFlags = static_cast<unsigned>(capacity) & 0x3fffffffu;
    if (&vector != &temporary) {
        WordVector old = vector;
        vector = temporary;
        temporary = old;
    }
    temporary.end = temporary.begin;
    if (temporary.allocator && temporary.begin)
        temporary.allocator->vtable->release(temporary.allocator, temporary.begin);
}
__forceinline bool allocateVector(WordVector& vector, Allocator* allocator, int capacity) {
    unsigned* storage = static_cast<unsigned*>(allocator->vtable->allocate(
        allocator, static_cast<unsigned>(capacity) * 4, 4));
    if (!storage) return false;
    replaceVector(vector, allocator, storage, capacity);
    return true;
}
void append(WordVector& vector, unsigned value) {
    int capacity = static_cast<int>(vector.capacityAndFlags & 0x3fffffffu);
    int size = vector.end - vector.begin;
    if (capacity <= size) {
        int newCapacity = capacity ? capacity * 2 : 1;
        if (capacity < newCapacity) {
            if (!vector.allocator || (vector.capacityAndFlags & 0xc0000000u) != 0x40000000u)
                return;
            unsigned* storage = static_cast<unsigned*>(vector.allocator->vtable->allocate(
                vector.allocator, static_cast<unsigned>(newCapacity) * 4, 32));
            unsigned* destination = storage;
            unsigned* source = vector.begin;
            int count = 0;
            if (source != vector.end) {
                do {
                    if (destination) *destination = *source;
                    ++source;
                    ++destination;
                } while (source != vector.end);
                count = vector.end - vector.begin;
            }
            if (vector.begin)
                vector.allocator->vtable->release(vector.allocator, vector.begin);
            vector.begin = storage;
            vector.end = storage + count;
            vector.capacityAndFlags = (vector.capacityAndFlags & 0xc0000000u) | newCapacity;
        }
    }
    if (vector.end) *vector.end = value;
    ++vector.end;
}
Blend* createBlend(Allocator* allocator, int capacity, signed char mode) {
    Blend* blend = static_cast<Blend*>(allocator->vtable->allocate(allocator, sizeof(Blend), 4));
    if (!blend) return 0;
    blend->vtable = reinterpret_cast<EvaluatorVtable*>(const_cast<unsigned*>(dat_003D8008));
    blend->allocator = allocator;
    blend->model = 0;
    blend->kind = 2;
    resetVector(blend->children);
    blend->vtable = reinterpret_cast<EvaluatorVtable*>(const_cast<unsigned*>(dat_003D82A4));
    resetVector(blend->weights);
    resetVector(blend->cachedWeights);
    blend->componentMode = 0;
    blend->dirty = 0;
    blend->normalize = 1;
    blend->vtable = reinterpret_cast<EvaluatorVtable*>(const_cast<unsigned*>(dat_003D8500));
    if (!allocateVector(blend->children, blend->allocator, capacity)) return blend;
    if (!allocateVector(blend->weights, blend->allocator, capacity)) return blend;
    for (int i = 0; i < capacity; ++i) {
        float* output = reinterpret_cast<float*>(blend->weights.end++);
        if (output) *output = i == 0 ? 1.0f : 0.0f;
    }
    if (!allocateVector(blend->cachedWeights, blend->allocator, capacity)) return blend;
    for (int i = 0; i < capacity; ++i) {
        float* output = reinterpret_cast<float*>(blend->cachedWeights.end++);
        if (output) *output = i == 0 ? 1.0f : 0.0f;
    }
    blend->componentMode = mode;
    return blend;
}
Evaluator* createEvaluator(Allocator* allocator, void* resource, void* context, int maximum) {
    EvaluationArguments args;
    args.resource = resource;
    args.modelCount = *reinterpret_cast<int*>(*reinterpret_cast<unsigned char**>(
        static_cast<unsigned char*>(context) + 8) + 0x10);
    args.maximumTrackCount = maximum;
    args.useCache = 1;
    return fn_002A63D8(&args, allocator);
}
void configureEvaluator(Evaluator* evaluator, Player* player, void* context) {
    if (!(*reinterpret_cast<unsigned*>(skeletonResource(player->model) + 0x28) & 2))
        evaluator->disableTranslation = 1;
    evaluator->vtable->setModel(evaluator, context);
}
}

// NonMatching: full observed interval 001C440C..001C4C80, including its pools.
extern "C" Player* fn_001C440C(Player* self, Archive* archive, Model* model, Allocator* allocator, int count) {
    self = fn_0024E47C(self);
    self->vtable = dat_003D62A0;
    self->model = model;
    self->single = 0;
    self->blend = 0;
    self->evaluators.count = 0;
    self->evaluators.capacity = 0;
    self->evaluators.entries = 0;
    self->workspace = 0;
    Workspace* workspace = new Workspace;
    if (workspace)
        workspace = static_cast<Workspace*>(fn_001D956C(workspace, allocator,
            *reinterpret_cast<int*>(skeletonResource(self->model) + 0x18), self->model->heap));
    self->workspace = workspace;
    int animationCount = *reinterpret_cast<int*>(*archive->resource + 0x64);
    AnimationTable* table = new AnimationTable;
    if (table) table = static_cast<AnimationTable*>(fn_0024E598(table, animationCount));
    self->animationTable = table;
    int maximum = 0;
    for (int i = 0; i < animationCount; ++i) {
        unsigned char* dictionary = relative(*archive->resource + 0x68);
        unsigned char* record = dictionary ? relative(dictionary + 0x28 + i * 0x10) : 0;
        fn_0024E568(self->animationTable, reinterpret_cast<const char*>(relative(record + 8)), record);
        int tracks = *reinterpret_cast<int*>(record + 0x18);
        if (maximum < tracks) maximum = tracks;
    }
    void* context = model->context;
    void* resource = fn_0024E49C(allocator, "SkeletalAnimation");
    if (count > 1) {
        Blend* blend = createBlend(allocator, count, 0);
        self->blend = blend;
        blend->vtable->setModel(reinterpret_cast<Evaluator*>(blend), context);
        fn_0026AC60(&self->evaluators, count, fn_00293088(dat_003E23B8), 4);
        for (int i = 0; i < count; ++i) {
            Evaluator* evaluator = createEvaluator(allocator, resource, context, maximum);
            configureEvaluator(evaluator, self, context);
            if (self->evaluators.count < self->evaluators.capacity)
                self->evaluators.entries[self->evaluators.count++] = evaluator;
            append(self->blend->children, reinterpret_cast<unsigned>(evaluator));
        }
        BindingTable* bindings = self->model->bindings;
        int index = self->model->bindingIndex;
        if (index >= 0 && index < bindings->end - bindings->begin && bindings->stride > 0)
            bindings->entries[index * bindings->stride] = reinterpret_cast<Evaluator*>(self->blend);
    } else {
        self->single = createEvaluator(allocator, resource, context, maximum);
        configureEvaluator(self->single, self, context);
    }
    return self;
}

#endif // NON_MATCHING
