namespace {
struct Object {
    unsigned char pad[0x10];
    void* dispatch;
};
typedef void* (*Method)(void*);
struct Table { unsigned char pad[0x0c]; Method method; };
struct Dispatch { Table* table; };
}

extern "C" void* fn_0018F0E4(void* self) {
    Dispatch* d = static_cast<Dispatch*>(static_cast<Object*>(self)->dispatch);
    return d->table->method(d);
}

extern "C" void* fn_001DA27C(void* self) {
    Dispatch* d = static_cast<Dispatch*>(static_cast<Object*>(self)->dispatch);
    return d->table->method(d);
}

extern "C" void* fn_001DCAE0(void* self) {
    Dispatch* d = static_cast<Dispatch*>(static_cast<Object*>(self)->dispatch);
    return d->table->method(d);
}
