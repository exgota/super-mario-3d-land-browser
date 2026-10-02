namespace {
struct Inner {
    char pad[0xC];
    void* value;
};

struct Object {
    char pad[0x68];
    Inner* inner;
};
}

extern "C" void fn_0026F9E4(void*);
extern "C" void fn_0012E654(void*);

extern "C" void fn_0014221C(Object* self)
{
    void* value = self->inner->value;
    if (value)
        fn_0026F9E4(value);
}

extern "C" void fn_00142628(Object* self)
{
    void* value = self->inner->value;
    if (value)
        fn_0012E654(value);
}
