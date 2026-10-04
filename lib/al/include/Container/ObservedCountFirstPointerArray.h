#pragma once
#include <nn/types.h>

namespace observed_containers {

// Count-first storage used by 0026AC60, distinct from the established
// capacity-first sead::PtrArray. This is not an original nominal class claim.
struct CountFirstPointerArrayStorage {
    int count;
    int capacity;
    void** entries;

    CountFirstPointerArrayStorage() : count(0), capacity(0), entries(0) {}
};

template <typename T>
struct CountFirstPointerArray : CountFirstPointerArrayStorage {
    int append(T* entry) {
        int limit = capacity;
        int size = count;
        if (size < limit) {
            entries[size] = entry;
            return ++count;
        }
        return size;
    }
};

static_assert(sizeof(CountFirstPointerArrayStorage) == 12, "observed storage size");
static_assert(offsetof(CountFirstPointerArrayStorage, count) == 0, "count first");
static_assert(offsetof(CountFirstPointerArrayStorage, capacity) == 4, "capacity second");
static_assert(offsetof(CountFirstPointerArrayStorage, entries) == 8, "entry buffer");
static_assert(sizeof(CountFirstPointerArray<void>) == 12, "typed view adds no fields");

} // namespace observed_containers

extern "C" void fn_0026AC60(observed_containers::CountFirstPointerArrayStorage*,
                           int capacity, void* heap, int alignment);
