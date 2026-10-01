#include <Resource/ResourceTagBuilder230794.h>

using namespace dot230794;

extern "C" {
void fn_00231A98(SizeAccumulator*, const Resource*, Options);
void fn_002AF598(SizeAccumulator*, const Resource*, ExtendedOptions);
void fn_0029D100(SizeAccumulator*, const Resource*, const ModelOptions*);
void fn_0029D798(SizeAccumulator*, const Resource*, const ModelOptions*);
void fn_0029A968(SizeAccumulator*, const Resource*, Options);
void fn_002A1CC4(SizeAccumulator*, const Resource*, const Options*);
void fn_0029C4AC(SizeAccumulator*, const Resource*, Options);
void fn_00299DF4(SizeAccumulator*, const Resource*, Options);
void fn_002A1644(SizeAccumulator*, const Resource*, Options);
void fn_002A89D4(SizeAccumulator*, const Resource*, Options);
void fn_002B1064(SizeAccumulator*, const Resource*, Options);
void fn_002319F4(SizeAccumulator*, const Resource*, bool);
void fn_002A6E54(SizeAccumulator*, const Resource*);
Object* fn_002B4A90(void*, const Resource*, const Options*, void*);
Object* fn_002A0F0C(void*, const Resource*, const Options*, void*);
Object* fn_002AFBA8(void*, const Resource*, const ExtendedOptions*, void*);
Object* fn_0029FFFC(const ModelOptions*, void*, const Resource*, void*);
Object* fn_0029D854(void*, const Resource*, const ModelOptions*, void*, void*);
Object* fn_002A201C(void*, const Resource*, const Options*, void*);
Object* fn_0029C8DC(void*, const Resource*, const Options*, void*);
Object* fn_00299F0C(void*, const Resource*, const Options*, void*);
Object* fn_0029AA80(void*, const Resource*, const Options*, void*);
Object* fn_002A175C(void*, const Resource*, const Options*, void*);
Object* fn_002A8B0C(void*, const Resource*, const Options*, void*);
Object* fn_002B13E8(void*, const Resource*, const Options*, void*);
Object* fn_002A75F4(const Resource*, Word*, void*);
void fn_00338920(const Builder*, SizeAccumulator*, SizeAccumulator*, Object*,
                const Resource*, void*, void*, bool);
extern TypeInfo dat_003F0730;
}

namespace
{
inline Word field(const void* record, Word offset)
{
    return *reinterpret_cast<const Word*>(static_cast<const unsigned char*>(record) + offset);
}
inline const Resource* relative(const void* record, Word offset)
{
    const unsigned char* location = static_cast<const unsigned char*>(record) + offset;
    int displacement = *reinterpret_cast<const int*>(location);
    return displacement ? reinterpret_cast<const Resource*>(location + displacement) : 0;
}
inline void reserve(SizeAccumulator* size, Word bytes)
{
    size->size = ((size->size + size->alignment - 1) & ~(size->alignment - 1)) + bytes;
}
inline void reserveAligned(SizeAccumulator* size, Word bytes)
{
    Word previous = size->alignment;
    size->alignment = 32;
    if (size->maximumAlignment < 32) size->maximumAlignment = 32;
    reserve(size, bytes);
    size->alignment = previous;
    if (size->maximumAlignment < previous) size->maximumAlignment = previous;
}
inline void reserveList(SizeAccumulator* size, Word count)
{
    reserve(size, 0x14);
    if (count) reserve(size, count * 4);
}
inline void setOptions(Options& options, const Builder* builder, bool preserveOne = false)
{
    options.enabled = builder->enabled;
    options.count0c = builder->count0c;
    options.count08 = builder->count08;
    if (!preserveOne) options.count1c = builder->count1c;
    options.auxiliary = builder->includeAuxiliary;
}
inline void setExtended(ExtendedOptions& options, const Builder* builder, bool preserveOne = false)
{
    setOptions(options.base, builder, preserveOne);
    options.value10 = builder->value10;
    options.value14 = builder->value14;
    options.value18 = builder->value18;
}
inline void reserveBase(SizeAccumulator* size, const Resource* resource, Options options)
{
    reserveAligned(size, options.count08 * 4);
    reserveList(size, options.count0c);
    if (options.auxiliary && field(resource, 0x28))
    {
        Word count = field(resource, 0x28);
        reserve(size, 0x2c);
        reserve(size, count * 4);
        reserve(size, count * options.count1c * 4);
    }
}
inline void reserveModel(SizeAccumulator* size, const Resource* resource, ModelOptions options)
{
    const Resource* skeleton = relative(resource, 0xe0);
    reserveAligned(size, field(skeleton, 0x18) * 64);
    reserve(size, 0x60);
    reserveAligned(size, field(skeleton, 0x18) * 64);
    reserveAligned(size, field(skeleton, 0x18) * 48);
    reserveAligned(size, field(skeleton, 0x18) * 48);
    reserveList(size, options.common.base.count0c);
    reserveList(size, options.common.base.count0c);
    reserve(size, 0x234);
    fn_002AF598(size, resource, options.common);
    if (options.common.base.auxiliary)
    {
        int count = static_cast<int>(field(resource, 0x28));
        for (int i = 0; i < count; ++i)
        {
            const Resource* dictionary = relative(resource, 0x2c);
            const Resource* item = dictionary ? relative(dictionary, 0x28 + i * 16) : 0;
            if ((field(item, 4) & 1) && field(item, 0xc) == 1)
            {
                reserve(size, 0x68);
                fn_002319F4(size, item, true);
                break;
            }
        }
    }
}
}

// NonMatching proposal. Canonical ARMCC checking, not this comment, awards O.
#ifdef NON_MATCHING
extern "C" Object* fn_00230794(const Builder* builder, SizeAccumulator* primary,
    SizeAccumulator* secondary, void* context, const Resource* resource,
    void* argument5, void* argument6, bool recurse, bool estimate)
{
    if (!resource) return 0;
    Object* result = 0;
    switch (field(resource, 0))
    {
    case 0x40000000:
    {
        Options options;
        setOptions(options, builder);
        if (estimate) {
            if (primary) { reserve(primary, 0x4c); reserveBase(primary, resource, options); }
        } else result = fn_002B4A90(context, resource, &options, argument5);
        break;
    }
    case 0x40000002:
    {
        Options options;
        setOptions(options, builder);
        if (estimate) {
            if (primary) { reserve(primary, 0x148); fn_00231A98(primary, resource, options); }
        } else result = fn_002A0F0C(context, resource, &options, argument5);
        break;
    }
    case 0x40000012:
    {
        ExtendedOptions options;
        setExtended(options, builder);
        if (estimate) {
            if (primary) { reserve(primary, 0x220); fn_002AF598(primary, resource, options); }
        } else result = fn_002AFBA8(context, resource, &options, argument5);
        break;
    }
    case 0x40000112:
    {
        ModelOptions options;
        const Resource* checked = (field(resource, 0) & 0x40000112) == 0x40000112 ? resource : 0;
        setExtended(options.common, builder, true);
        options.common.base.count08 = field(checked, 0x20) + field(checked, 0xe0) + builder->extra20;
        options.extra = field(checked, 0xe0) + builder->extra20;
        if (estimate) {
            if (primary) fn_0029D100(primary, checked, &options);
            if (secondary) fn_0029D798(secondary, checked, &options);
        } else result = fn_0029D854(context, resource, &options, argument5, argument6);
        break;
    }
    case 0x40000092:
    {
        ModelOptions options;
        setExtended(options.common, builder);
        if (estimate) {
            if (primary) reserveModel(primary, resource, options);
        } else result = fn_0029FFFC(&options, context, resource, argument5);
        break;
    }
    case 0x40000006:
    {
        Options options;
        setOptions(options, builder, true);
        if (estimate) {
            if (primary) fn_002A1CC4(primary, resource, &options);
        } else result = fn_002A201C(context, resource, &options, argument5);
        break;
    }
    case 0x400000A2:
    {
        Options options;
        setOptions(options, builder);
        if (estimate) {
            if (primary) fn_0029C4AC(primary, resource, options);
        } else result = fn_0029C8DC(context, resource, &options, argument5);
        break;
    }
    case 0x40000222:
    {
        Options options;
        setOptions(options, builder);
        if (estimate) {
            if (primary) fn_00299DF4(primary, resource, options);
        } else result = fn_00299F0C(context, resource, &options, argument5);
        break;
    }
    case 0x40000422:
    {
        Options options;
        setOptions(options, builder);
        if (estimate) {
            if (primary) fn_0029A968(primary, resource, options);
        } else result = fn_0029AA80(context, resource, &options, argument5);
        break;
    }
    case 0x40000122:
    {
        Options options;
        setOptions(options, builder);
        if (estimate) {
            if (primary) fn_002A1644(primary, resource, options);
        } else result = fn_002A175C(context, resource, &options, argument5);
        break;
    }
    case 0x40000042:
    {
        Options options;
        setOptions(options, builder, true);
        if (estimate) {
            if (primary) fn_002A89D4(primary, resource, options);
        } else result = fn_002A8B0C(context, resource, &options, argument5);
        break;
    }
    case 0x4000000A:
    {
        Options options;
        setOptions(options, builder);
        if (estimate) {
            if (primary) fn_002B1064(primary, resource, options);
        } else result = fn_002B13E8(context, resource, &options, argument5);
        break;
    }
    case 0x00800000:
        if (estimate) {
            if (primary) fn_002A6E54(primary, resource);
        } else {
            Word temporary;
            result = fn_002A75F4(resource, &temporary, argument5);
        }
        break;
    }
    if (estimate)
    {
        if (recurse) fn_00338920(builder, primary, secondary, 0, resource, 0, 0, true);
    }
    else
    {
        Object* derived = 0;
        if (result)
        {
            const TypeInfo* type = result->vtable->type(result);
            do
            {
                if (type == &dat_003F0730) { derived = result; break; }
                type = type->parent;
            } while (type);
        }
        if (derived && recurse)
            fn_00338920(builder, 0, 0, derived, resource, argument5, argument6, false);
    }
    return result;
}
#endif
