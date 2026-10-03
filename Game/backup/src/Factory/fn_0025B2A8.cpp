namespace {
struct StringTmp;
struct StringVtable {
    void (*slot0)(StringTmp*);
    void (*slot1)(StringTmp*);
    void (*prepare)(StringTmp*);
};
struct StringTmp {
    StringVtable* vtable;
    const char* text;
    char storage[36];
};
struct Entry {
    char unknown0[8];
    void* flags;
    char unknownC[20];
};
struct SaveData {
    char unknown0[48];
    unsigned count;
    Entry* entries;
    char unknown38[8];
    short coinCount;

    const Entry& getEntry(unsigned index) const {
        if (count > index)
            return entries[index];
        return entries[0];
    }
};

template<typename T>
inline T clamp(T value, const T& lower, const T& upper) {
    if (value < lower)
        value = lower;
    else if (upper < value)
        value = upper;
    return value;
}
}

extern "C" StringTmp* _ZN2al9StringTmpILi32EEC1EPKcz(StringTmp*, const char*, ...);
extern "C" bool fn_0026B04C(void*, const char*);
extern "C" void fn_0026B434(void*, const char*);
extern "C" const char dat_003A2958[];

extern "C" void fn_0025B2A8(SaveData* self, unsigned index, int coin) {
    StringTmp second;
    StringTmp first;
    StringTmp* name = _ZN2al9StringTmpILi32EEC1EPKcz(&first, dat_003A2958, coin);
    name->vtable->prepare(name);
    const char* text = name->text;
    if (!fn_0026B04C(self->getEntry(index).flags, text)) {
        self->coinCount = clamp(self->coinCount + 1, 0, 999);
    }
    name = _ZN2al9StringTmpILi32EEC1EPKcz(&second,
        "\x83\x52\x83\x8c\x83\x4e\x83\x67\x83\x52\x83\x43\x83\x93\x8e\xe6\x93\xbe\x8d\xcf[%d]", coin);
    name->vtable->prepare(name);
    Entry* entry = self->count > index ? self->entries + index : self->entries;
    fn_0026B434(entry->flags, name->text);
}
