#include <nn/types.h>
namespace observed_relative_resource_release {
template<class T> struct RelativePointer {
    s32 displacement;
    T* get() {
        return displacement
            ? reinterpret_cast<T*>(reinterpret_cast<u8*>(&displacement) + displacement)
            : 0;
    }
};
struct Child {
    u8 unknown00[8];
    RelativePointer<void> allocation;
};
struct Table { RelativePointer<Child> entries[6]; };
struct Resource {
    u8 unknown00[0x28];
    RelativePointer<Table> children;
};
typedef void* (*Release)(void*, void*);
struct DispatchPrefix {
    void* unknown00[3];
    Release release;
};
struct AllocatorPrefix { const DispatchPrefix* dispatch; };
static_assert(sizeof(RelativePointer<void>) == 4, "relative displacement field");
static_assert(sizeof(Table) == 24, "six independently observed entries");
static_assert(offsetof(Resource, children) == 0x28, "resource table offset");
static_assert(offsetof(Child, allocation) == 8, "inner allocation offset");
static_assert(offsetof(DispatchPrefix, release) == 0xc, "proved live release slot");
}

extern "C" void* fn_002B2BF8(
        observed_relative_resource_release::AllocatorPrefix* allocator,
        observed_relative_resource_release::Resource* resource) {
    using namespace observed_relative_resource_release;
    if (!resource) return allocator;
    Table* table = resource->children.get();
    if (table) {
        for (unsigned i = 0; i < 6; ++i) {
            if (table->entries[i].get()) {
                allocator->dispatch->release(allocator, table->entries[i].get()->allocation.get());
                // The callback can change the relative entry and dispatch.
                allocator->dispatch->release(allocator, table->entries[i].get());
            }
        }
        allocator->dispatch->release(allocator, table);
    }
    return allocator->dispatch->release(allocator, resource);
}
