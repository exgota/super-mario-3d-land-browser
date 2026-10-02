namespace {
struct Inner {
    void *vtable;
};
struct Middle {
    unsigned char pad[0x10];
    Inner *inner;
};
struct Outer {
    unsigned int unused;
    Middle *middle;
};
typedef void (*Dispatch)(void *);
}

extern "C" void fn_00173FC4(void *p) {
    void *q = *(void **)((char *)p + 4);
    q = *(void **)((char *)q + 0x10);
    return ((Dispatch)(*(void ***)q)[2])(q);
}
extern "C" void fn_001963FC(void *p) {
    void *q = *(void **)((char *)p + 4);
    q = *(void **)((char *)q + 0x10);
    return ((Dispatch)(*(void ***)q)[2])(q);
}
extern "C" void fn_00197848(void *p) {
    void *q = *(void **)((char *)p + 4);
    q = *(void **)((char *)q + 0x10);
    return ((Dispatch)(*(void ***)q)[2])(q);
}
extern "C" void fn_001B7A18(void *p) {
    void *q = *(void **)((char *)p + 4);
    q = *(void **)((char *)q + 0x10);
    return ((Dispatch)(*(void ***)q)[2])(q);
}
extern "C" void fn_002592E0(void *p) {
    void *q = *(void **)((char *)p + 4);
    q = *(void **)((char *)q + 0x10);
    return ((Dispatch)(*(void ***)q)[2])(q);
}
