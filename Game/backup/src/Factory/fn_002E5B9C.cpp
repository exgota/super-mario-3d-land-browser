namespace {
struct Object {
    unsigned char pad[0x878];
    void* value;
};
}

extern "C" unsigned fn_002EA8E4(void*);

extern "C" unsigned fn_002E5B9C(Object* self) {
    return fn_002EA8E4(self->value);
}
