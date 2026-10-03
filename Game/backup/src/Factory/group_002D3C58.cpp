namespace {
struct DispatchObject {
    unsigned int reserved;
    void* object;
};

struct DispatchVTable {
    unsigned int reserved[5];
    void* (*call)(void*);
};
}

extern "C" void* fn_002D3C58(DispatchObject* self) {
    DispatchVTable* table = *reinterpret_cast<DispatchVTable**>(self->object);
    return table->call(self->object);
}

extern "C" void* fn_003764A8(DispatchObject* self) {
    DispatchVTable* table = *reinterpret_cast<DispatchVTable**>(self->object);
    return table->call(self->object);
}
