// Clean-room reconstruction of EU 0x00298C58 from the executable.
// The class/module identity is unresolved. These names describe observed roles.
// NonMatching: diagnostic address-based root, not an accepted class identity.
// Placement in the SDK/902 build is a provisional compiler hypothesis.
#ifdef NON_MATCHING
#include <retail/ResourceParticleFamily.h>
namespace resource298c58
{
struct Allocator;
struct AllocatorTable
{
    void (*slot0)();
    void (*slot4)();
    void* (*allocate)(Allocator*, unsigned, int);
    void (*release)(Allocator*, void*);
};
struct Allocator
{
    const AllocatorTable* table;
    void* allocate(unsigned size, int alignment) { return table->allocate(this, size, alignment); }
    void release(void* memory) { table->release(this, memory); }
};

template<class T> struct Vector
{
    Allocator* allocator;
    T* begin;
    T* end;
    unsigned capacityAndMode;

    int size() const { return end - begin; }
    int capacity() const { return capacityAndMode & 0x3fffffff; }
    bool reserve(int count)
    {
        if (capacity() >= count)
            return true;
        if (!allocator || capacityAndMode >> 30 != 1)
            return false;
        T* buffer = static_cast<T*>(allocator->allocate(count * sizeof(T), 32));
        int oldSize = 0;
        T* source = begin;
        T* last = end;
        if (source != last)
        {
            T* destination = buffer;
            do
            {
                if (destination)
                    *destination = *source;
                ++source;
                ++destination;
            } while (source != last);
            oldSize = size();
        }
        if (begin)
            allocator->release(begin);
        begin = buffer;
        end = buffer + oldSize;
        capacityAndMode = (capacityAndMode & 0xc0000000) | count;
        return true;
    }
    bool append(const T& value)
    {
        bool possible = true;
        if (capacity() <= size())
            possible = reserve(capacity() ? 2 * capacity() : 1);
        if (possible)
        {
            if (end)
                *end = value;
            ++end;
        }
        return possible;
    }
    void swap(Vector& other)
    {
        if (this == &other)
            return;
        T* previousBegin = begin;
        begin = other.begin;
        other.begin = previousBegin;
        T* previousEnd = end;
        end = other.end;
        other.end = previousEnd;
        unsigned previousCapacity = capacityAndMode;
        capacityAndMode = other.capacityAndMode;
        other.capacityAndMode = previousCapacity;
        Allocator* previousAllocator = allocator;
        allocator = other.allocator;
        other.allocator = previousAllocator;
    }
    ~Vector()
    {
        for (int i = 1; i < size() + 1; ++i)
            (end - i)->~T();
        end = begin;
        if (allocator && begin)
            allocator->release(begin);
    }
};

struct ResourceEntry
{
    unsigned type;
    unsigned char copy;
    unsigned char padding5[3];
    int binding;
    int nestedOffset;
    unsigned word10;
};
struct Entry24
{
    bool owned;
    ResourceEntry* resource;
    unsigned zero;
    unsigned type;
    void* first;
    void* second;
};
struct Entry28
{
    bool owned;
    ResourceEntry* resource;
    unsigned zero;
    unsigned type;
    void* first;
    void* second;
    unsigned nested;
};
struct Resource
{
    unsigned flags;
    unsigned unknown04[11];
    int bindingOffset;
    int count24;
    int entries24Offset;
    int count28;
    int entries28Offset;
};
struct Options
{
    unsigned words[4];
    int capacity24;
    int capacity28;
};
struct Object;
struct ObjectTable
{
    void (*destroy)(Object*);
    void (*unknown04[8])();
    int (*initialize)(Object*, Allocator*);
};
struct Object
{
    const ObjectTable* table;
    Allocator* allocator;
    Resource* resource;
    Object* parent;
    Object* owner;
    Vector<Object*> children;
    unsigned unknown24[12];
    Vector<Entry24> entries24;
    Vector<Entry28> entries28;
    float defaultOneVector74[3];
    float defaultZeroVector80[3];
    bool special;
};
struct Bindings
{
    unsigned char unknown00[0xd0];
    unsigned char reverse;
    unsigned char enabled[0x13];
    void* pointers[1];
    void* primary(int index) const { return enabled[index] ? pointers[index * 2] : 0; }
    void* secondary(int index) const { return enabled[index] ? pointers[index * 2 + 1] : 0; }
    void* selected(int index) const { return enabled[index] ? pointers[index * 2 + (reverse ? 0 : 1)] : 0; }
};

static inline void* relative(int* location)
{
    return *location ? reinterpret_cast<char*>(location) + *location : 0;
}
static inline char* align4(char* address)
{
    return reinterpret_cast<char*>((reinterpret_cast<unsigned>(address) + 3) & ~3u);
}
static inline void destroy(Object* object)
{
    if (object)
    {
        object->table->destroy(object);
        object->allocator->release(object);
    }
}
template<class T> static inline void bind(Vector<T>& entries, Bindings* bindings)
{
    for (T* entry = entries.begin; entry != entries.end; ++entry)
    {
        int index = entry->resource->binding;
        if (index < 0)
        {
            entry->first = 0;
            entry->second = 0;
        }
        else if (bindings->reverse)
        {
            entry->second = bindings->secondary(index);
            entry->first = bindings->selected(index);
        }
        else
        {
            entry->first = bindings->primary(index);
            entry->second = bindings->selected(index);
        }
    }
}
}
using namespace resource298c58;
extern "C" Object* fn_002997C8(Object*, Allocator*, Resource*, const Options*);
extern "C" bool fn_00231EF0(Object*, Object*);
extern "C" ResourceEntry* fn_0029B57C(ResourceEntry**, Allocator*);
extern "C" ResourceEntry* fn_0029AFE8(ResourceEntry**, Allocator*);
extern "C" void fn_0029E620(void*, Object*);

extern "C" Object* fn_00298C58(Object* owner, Resource* resource, const Options* options,
                              Allocator* allocator, void* parameter4, void* parameter5)
{
    Resource* input = resource;
    if (!input || (input->flags & 0x40000001) != 0x40000001)
        input = 0;
    int capacity24 = options->capacity24;
    int capacity28 = options->capacity28;
    if (input->count24 > capacity24)
        capacity24 = input->count24;
    if (input->count28 > capacity28)
        capacity28 = input->count28;
    unsigned size = 0x90;
    size += capacity24 * sizeof(Entry24);
    size = (size + 3) & ~3u;
    size += capacity28 * sizeof(Entry28);
    char* memory = static_cast<char*>(allocator->allocate(size, 4));
    if (!memory)
        return 0;
    Object* object = fn_002997C8(reinterpret_cast<Object*>(memory), allocator, input, options);
    char* cursor = memory + 0x90;
    if (!((object->table->initialize(object, allocator) >> 31) + 1))
    {
        destroy(object);
        return 0;
    }
    if (owner && (!owner->parent || (owner->parent != object && !fn_00231EF0(owner->parent, object))) &&
        !object->parent)
    {
        if (owner->children.append(object))
            object->parent = owner->owner;
    }
    {
        Vector<Entry24> storage;
        storage.allocator = 0;
        storage.begin = storage.end = reinterpret_cast<Entry24*>(align4(cursor));
        storage.capacityAndMode = capacity24 & 0x3fffffff;
        object->entries24.swap(storage);
    }
    cursor = align4(cursor) + capacity24 * sizeof(Entry24);
    bool success = true;
    int* entries24 = static_cast<int*>(relative(&input->entries24Offset));
    int* end24 = entries24 + input->count24;
    while (entries24 != end24)
    {
        ResourceEntry* entry = static_cast<ResourceEntry*>(relative(entries24++));
        Entry24 value;
        if (entry->copy)
        {
            value.owned = true;
            value.resource = fn_0029B57C(&entry, allocator);
            if (!value.resource)
                success = false;
        }
        else
        {
            value.owned = false;
            value.resource = entry;
        }
        value.zero = 0;
        value.type = entry->type;
        value.first = object;
        value.second = object;
        object->entries24.append(value);
    }
    {
        Vector<Entry28> storage;
        storage.allocator = 0;
        storage.begin = storage.end = reinterpret_cast<Entry28*>(align4(cursor));
        storage.capacityAndMode = capacity28 & 0x3fffffff;
        object->entries28.swap(storage);
    }
    int* entries28 = static_cast<int*>(relative(&input->entries28Offset));
    int* end28 = entries28 + input->count28;
    while (entries28 != end28)
    {
        ResourceEntry* entry = static_cast<ResourceEntry*>(relative(entries28++));
        Entry28 value;
        if (entry->copy)
        {
            value.owned = true;
            value.resource = fn_0029AFE8(&entry, allocator);
            if (!value.resource)
                success = false;
        }
        else
        {
            value.owned = false;
            value.resource = entry;
        }
        value.zero = 0;
        value.type = entry->type;
        value.nested = false;
        if (entry->type == 0x800002 || entry->type == 0x800001 || entry->type == 0x800004)
        {
            char* nested = static_cast<char*>(relative(&entry->nestedOffset));
            if (nested && relative(reinterpret_cast<int*>(nested + 0x20)))
                value.nested = true;
        }
        if (entry->type == 0x80000 && (entry->word10 & 0xff) == 1)
            object->special = true;
        object->entries28.append(value);
    }
    if (!success)
    {
        destroy(object);
        return 0;
    }
    Bindings* bindings = static_cast<Bindings*>(fn_002A451C(object, relative(&input->bindingOffset), allocator, parameter4, parameter5));
    if (!bindings)
    {
        destroy(object);
        return 0;
    }
    fn_0029E620(parameter5, object);
    bind(object->entries24, bindings);
    bind(object->entries28, bindings);
    return object;
}

#endif // NON_MATCHING
