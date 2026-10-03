namespace {
struct DispatchObject;
struct DispatchTable {
    unsigned int unused0;
    unsigned int unused1;
    int (*call)(DispatchObject *);
};

struct DispatchObject {
    DispatchTable *table;
};

struct Wrapper {
    unsigned int unused;
    DispatchObject *object;
};
}

extern "C" void fn_001B9F8C(Wrapper *self) {
    self->object->table->call(self->object);
}

extern "C" void fn_002D50E4(Wrapper *self) {
    self->object->table->call(self->object);
}
