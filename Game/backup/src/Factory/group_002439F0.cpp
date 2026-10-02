namespace {
struct Ref {
    void* value;
};

struct Entry {
    unsigned char pad[0x28];
    Ref* ref;
};

}

extern "C" void* fn_00243A00(void*);
extern "C" void* fn_0024AD3C(void*);
extern "C" void* fn_0026252C(void*);

extern "C" void* fn_002439F0(void* self) {
    Entry* entry = static_cast<Entry*>(self);
    void* first = entry->ref->value;
    void* next = *reinterpret_cast<void**>(static_cast<unsigned char*>(first) + 0x2C);
    return fn_00243A00(next);
}

extern "C" void* fn_0024AD2C(void* self) {
    Entry* entry = static_cast<Entry*>(self);
    void* first = entry->ref->value;
    void* next = *reinterpret_cast<void**>(static_cast<unsigned char*>(first) + 0x2C);
    return fn_0024AD3C(next);
}

extern "C" void* fn_0026251C(void* self) {
    Entry* entry = static_cast<Entry*>(self);
    void* first = entry->ref->value;
    void* next = *reinterpret_cast<void**>(static_cast<unsigned char*>(first) + 0x2C);
    return fn_0026252C(next);
}
