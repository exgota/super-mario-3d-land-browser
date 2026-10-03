namespace {
struct Object { unsigned char pad[4]; void *next; };
struct Inner { unsigned char pad[0x1c]; void **vtable; };
}

extern "C" void fn_00196824(void *self) {
    Object *a = static_cast<Object *>(self);
    Inner *b = static_cast<Inner *>(a->next);
    void *c = *reinterpret_cast<void **>(reinterpret_cast<char *>(b) + 0x1c);
    void **v = *reinterpret_cast<void ***>(c);
    reinterpret_cast<void (*)(void *)>(v[1])(c);
}

extern "C" void fn_00196EA0(void *self) {
    Object *a = static_cast<Object *>(self);
    Inner *b = static_cast<Inner *>(a->next);
    void *c = *reinterpret_cast<void **>(reinterpret_cast<char *>(b) + 0x1c);
    void **v = *reinterpret_cast<void ***>(c);
    reinterpret_cast<void (*)(void *)>(v[1])(c);
}
