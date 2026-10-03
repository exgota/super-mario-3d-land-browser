namespace {
struct DispatchObject { void **vtable; };
struct Holder { unsigned int unused; DispatchObject *object; };
typedef int (*Method)(DispatchObject *);
}
extern "C" int fn_002D1F74(Holder *self) {
    DispatchObject *object = self->object;
    return reinterpret_cast<Method>(object->vtable[10])(object);
}
extern "C" int fn_002D2C64(Holder *self) {
    DispatchObject *object = self->object;
    return reinterpret_cast<Method>(object->vtable[10])(object);
}
