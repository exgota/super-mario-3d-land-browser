namespace {
typedef unsigned int u32;

struct Object {
    unsigned char pad0[0x68];
    u32 index;
    unsigned char pad1[0x74 - 0x6c];
    u32 count;
    unsigned char pad2[0x7c - 0x78];
    u32 *entries;
};
}

extern "C" u32 fn_0019C024(u32);
extern "C" u32 fn_00187F7C(u32);

extern "C" u32 fn_00212208(Object *self) {
    u32 index = self->index;
    u32 value = index < self->count ? self->entries[index] : 0;
    return fn_0019C024(value);
}

extern "C" u32 fn_00258048(Object *self) {
    u32 index = self->index;
    u32 value = index < self->count ? self->entries[index] : 0;
    return fn_00187F7C(value);
}
