namespace {
struct Object;
struct Interface {
    int (*call)(Object*);
};
struct Object {
    Interface* interface;
};
struct Holder {
    void* pad[5];
    Object* object;
};
}

extern "C" int fn_0032CB44(Holder* self) {
    return self->object->interface->call(self->object);
}
