namespace {
struct Object {
    unsigned char actor[0x3c];
    int index;
    bool active;
    unsigned char padding[3];
    void* condition;
};

struct StringTmp {
    void (**vtable)(StringTmp*);
    const char* text;
    unsigned char storage[40];
};
}

extern "C" {
int fn_0027BB10(void*);
bool fn_0027BAD0(void*);
StringTmp* _ZN2al9StringTmpILi32EEC1EPKcz(StringTmp*, const char*, ...);
void fn_0027BEA0(void*, const char*, int);
void fn_001BFA04(void*, const char*);
extern const char dat_003B96C8[];
extern const char* const dat_003F1CB4[];
}

extern "C" void fn_001370BC(Object* self, bool enabled) {
    const char* const* names = dat_003F1CB4;
    bool active = false;
    if (enabled && fn_0027BB10(self->condition) >= 1 && fn_0027BAD0(self->condition))
        active = true;
    if (active) {
        if (!self->active) {
            self->active = true;
            StringTmp storage;
            StringTmp* name = _ZN2al9StringTmpILi32EEC1EPKcz(
                &storage, dat_003B96C8, names[self->index - 1]);
            name->vtable[2](name);
            fn_0027BEA0(self->actor + 8, name->text, 0);
        }
    } else if (self->active) {
        self->active = active;
        StringTmp storage;
        StringTmp* name = _ZN2al9StringTmpILi32EEC1EPKcz(
            &storage, dat_003B96C8, names[self->index - 1]);
        name->vtable[2](name);
        fn_001BFA04(self->actor + 8, name->text);
    }
}
