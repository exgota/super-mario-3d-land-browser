namespace {
struct ForwardedObject {
    void* value;
};

struct Receiver {
    unsigned int unknown;
    ForwardedObject* forwarded;
};
}

extern "C" int fn_002589F8(void* value);

extern "C" int fn_0018124C(Receiver* self) {
    return fn_002589F8(self->forwarded->value);
}
