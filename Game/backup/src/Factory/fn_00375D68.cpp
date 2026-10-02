namespace {
struct Interface {
    virtual void first() = 0;
    virtual void second() = 0;
};

struct Object {
    char padding[0xdc];
    Interface* interface;
};
}

extern "C" void fn_00375D68(Object* self) {
    return self->interface->second();
}
