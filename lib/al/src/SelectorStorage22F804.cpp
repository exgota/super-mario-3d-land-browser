#include <Resource/SelectorStorage22F804.h>

using namespace dot22f804;
extern "C" {
int fn_0028A998(Word*);
extern Word dat_003F0698, dat_003F069C, dat_003F06A0, dat_003F06A4;
extern Word dat_003F06A8, dat_003F06AC, dat_003F06B0, dat_003F06B4;
extern Word dat_003F06B8, dat_003F06BC, dat_003F06C0, dat_003F06C4;
extern Descriptor dat_003F06C8, dat_003F06D0, dat_003F06D8, dat_003F06E0;
extern Descriptor dat_003F06E8, dat_003F06F0, dat_003F06F8, dat_003F0700;
extern Descriptor dat_003F0708, dat_003F0710, dat_003F0718;
extern DescriptorPairPrefix dat_0042A4B0;
extern const Word dat_003D9134[], dat_003D9110[], dat_003D9158[], dat_003D91A0[];
extern const Word dat_003D91C4[], dat_003D91F8[], dat_003D917C[], dat_003D8574[];
extern const Word dat_003D8550[], dat_003D84C4[], dat_003D8598[], dat_003D83CC[];
}

namespace {
inline void initialize(Word* guard, Descriptor* object, const void* table,
                       unsigned char flag4, unsigned char flag5)
{
    if (!(*guard & 1) && fn_0028A998(guard)) {
        object->vtable = table;
        object->flag4 = flag4;
        object->flag5 = flag5;
    }
}
inline Word descriptor(int selector)
{
    // Retail evaluates all twelve static guards before selecting one object.
    initialize(&dat_003F06C4, &dat_003F06C8, dat_003D9134, 1, 0);
    initialize(&dat_003F06C0, &dat_003F06D0, dat_003D9110, 0, 0);
    initialize(&dat_003F06BC, &dat_003F06D8, dat_003D9158, 1, 0);
    initialize(&dat_003F06B8, &dat_003F06E0, dat_003D91A0, 1, 0);
    initialize(&dat_003F06B4, &dat_003F06E8, dat_003D91C4, 1, 0);
    initialize(&dat_003F06B0, &dat_003F06F0, dat_003D91F8, 1, 0);
    initialize(&dat_003F06AC, &dat_003F06F8, dat_003D917C, 0, 0);
    initialize(&dat_003F06A8, &dat_003F0700, dat_003D8574, 1, 1);
    initialize(&dat_003F06A4, &dat_003F0708, dat_003D8550, 1, 1);
    initialize(&dat_003F06A0, &dat_003F0710, dat_003D84C4, 1, 1);
    initialize(&dat_003F069C, &dat_003F0718, dat_003D8598, 1, 1);
    if (!(dat_003F0698 & 1) && fn_0028A998(&dat_003F0698)) {
        dat_0042A4B0.base.vtable = dat_003D83CC;
        dat_0042A4B0.base.flag4 = dat_0042A4B0.base.flag5 = 1;
        dat_0042A4B0.extra.vtable = dat_003D8574;
        dat_0042A4B0.extra.flag4 = dat_0042A4B0.extra.flag5 = 1;
    }
    switch (selector) {
    case 0: return reinterpret_cast<Word>(&dat_003F06C8);
    case 1: return reinterpret_cast<Word>(&dat_003F06D0);
    case 2: return reinterpret_cast<Word>(&dat_003F06D8);
    case 3: return reinterpret_cast<Word>(&dat_003F06F0);
    case 4: return reinterpret_cast<Word>(&dat_0042A4B0);
    case 5: return reinterpret_cast<Word>(&dat_003F06E0);
    case 6: return reinterpret_cast<Word>(&dat_003F06E8);
    case 7: return reinterpret_cast<Word>(&dat_003F06F8);
    case 8: return reinterpret_cast<Word>(&dat_003F0700);
    case 9: return reinterpret_cast<Word>(&dat_003F0710);
    case 10: return reinterpret_cast<Word>(&dat_003F0708);
    case 11: return reinterpret_cast<Word>(&dat_003F0718);
    default: return 0;
    }
}
inline void replace(WordArray& array, Allocator* allocator, Word* memory, Word count)
{
    WordArray temporary;
    temporary.allocator = allocator;
    temporary.begin = temporary.end = memory;
    temporary.capacityAndFlags = count & 0x3fffffff;
    if (&array != &temporary) {
        WordArray previous = array;
        array = temporary;
        temporary = previous;
    }
    temporary.end = temporary.begin;
    if (temporary.allocator && temporary.begin)
        temporary.allocator->vtable->deallocate(temporary.allocator, temporary.begin);
}
inline void resize(WordArray& array, int count)
{
    int size = static_cast<int>(reinterpret_cast<Word>(array.end) - reinterpret_cast<Word>(array.begin)) >> 2;
    if (count < size) {
        if (count < 0) count = 0;
    } else if (static_cast<int>(array.capacityAndFlags & 0x3fffffff) < count) {
        if (array.allocator && (array.capacityAndFlags >> 30) == 1) {
            Word* memory = array.allocator->vtable->allocate(array.allocator, static_cast<Word>(count) * 4, 32);
            Word* destination = memory;
            Word* source = array.begin;
            Word* end = array.end;
            int copied = 0;
            if (source != end) {
                do {
                    if (destination) *destination = *source;
                    ++destination;
                    ++source;
                } while (source != end);
                copied = static_cast<int>(reinterpret_cast<Word>(array.end) - reinterpret_cast<Word>(array.begin)) >> 2;
            }
            if (array.begin) array.allocator->vtable->deallocate(array.allocator, array.begin);
            array.begin = memory;
            array.end = memory + copied;
            array.capacityAndFlags = (array.capacityAndFlags & 0xc0000000) | count;
        } else count = array.capacityAndFlags & 0x3fffffff;
    }
    array.end = array.begin + count;
}
inline bool allocate(WordArray& array, Allocator* allocator, int count)
{
    Word* memory = allocator->vtable->allocate(allocator, static_cast<Word>(count) * 4, 4);
    if (!memory) return false;
    replace(array, allocator, memory, count);
    return true;
}
}

// NonMatching: original builds retain static-initialization and array-layer artifacts.
#ifdef NON_MATCHING
extern "C" Word fn_0022F804(Storage* storage, bool includeOptional)
{
    int count = storage->resource->selectorCount;
    if (!allocate(storage->selectors, storage->allocator, count)) return 0x80000000;
    for (int i = 0; i < count; ++i) {
        const int* location = &storage->resource->selectorsRelative;
        const int* selectors = *location ? reinterpret_cast<const int*>(reinterpret_cast<const unsigned char*>(location) + *location) : 0;
        Word value = descriptor(selectors[i]);
        Word* destination = storage->selectors.end++;
        if (destination) *destination = value;
    }
    count = storage->resource->valueCount;
    if (!allocate(storage->values0, storage->allocator, count)) return 0x80000000;
    resize(storage->values0, count);
    if (!allocate(storage->values1, storage->allocator, count)) return 0x80000000;
    resize(storage->values1, count);
    if (!allocate(storage->values2, storage->allocator, count)) return 0x80000000;
    resize(storage->values2, count);
    if (includeOptional) {
        if (!allocate(storage->optionalValues, storage->allocator, count)) return 0x80000000;
        resize(storage->optionalValues, count);
    }
    return 0;
}
#endif
