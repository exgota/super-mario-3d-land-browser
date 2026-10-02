namespace {
struct LookupObject {
    unsigned char padding[0x3C];
    void** entries;
};
}

extern "C" void fn_0019A938(void*, unsigned int);
extern "C" void fn_002CF0D8(void*, unsigned int);

extern "C" void fn_0015A614(LookupObject* object, unsigned int index) {
    void* entry = object->entries[index];
    if (entry != 0)
        fn_0019A938(entry, index);
}

extern "C" void fn_003288A0(LookupObject* object, unsigned int index) {
    void* entry = object->entries[index];
    if (entry != 0)
        fn_002CF0D8(entry, index);
}
