namespace {
struct Object {
    unsigned int reserved;
    void* field;
};
}

extern "C" int fn_00267C1C(void*);
extern "C" int fn_00253420(void*);

extern "C" int fn_001C94B8(Object* self) {
    void* value = self->field;
    if (value == 0)
        return -1;
    return fn_00267C1C(value);
}

extern "C" int fn_001C9508(Object* self) {
    void* value = self->field;
    if (value == 0)
        return -1;
    return fn_00253420(value);
}
