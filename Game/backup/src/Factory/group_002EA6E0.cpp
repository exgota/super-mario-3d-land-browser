namespace {
struct Receiver {
    unsigned char pad[0x878];
    void* value;
};
}

extern "C" int fn_002EA6E8(void*);
extern "C" int fn_002EA8E4(void*);
extern "C" int fn_002EA928(void*);

extern "C" int fn_002EA6E0(Receiver* self) {
    return fn_002EA6E8(self->value);
}

extern "C" int fn_002EA8DC(Receiver* self) {
    return fn_002EA8E4(self->value);
}

extern "C" int fn_002EA920(Receiver* self) {
    return fn_002EA928(self->value);
}
