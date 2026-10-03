namespace {
struct DispatchObject {
    void **vtable;
};

struct Owner {
    unsigned char pad[0x20];
    DispatchObject *object;
};

typedef int (*Dispatch)(DispatchObject *);
}

extern "C" int fn_0013DE84(Owner *self)
{
    return ((Dispatch)self->object->vtable[1])(self->object);
}

extern "C" int fn_0018EB94(Owner *self)
{
    return ((Dispatch)self->object->vtable[1])(self->object);
}
