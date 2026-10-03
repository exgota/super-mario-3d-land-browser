namespace {
struct Object {
    const void* vtable;
    unsigned char unknown[0x54];
    void* context;
};
}

extern "C" Object* fn_00258C58(Object*, void*, void*);
extern "C" Object* fn_00171D64(Object*, void*, void*);
extern "C" const unsigned char dat_003CF0E8[];
extern "C" const unsigned char dat_003CF270[];

extern "C" Object* fn_001963DC(Object* self, void* arg1, void* arg2, void* context) {
    Object* result = fn_00258C58(self, arg1, arg2);
    result->vtable = dat_003CF0E8;
    result->context = context;
    return result;
}

extern "C" Object* fn_00197C64(Object* self, void* arg1, void* arg2, void* context) {
    Object* result = fn_00171D64(self, arg1, arg2);
    result->vtable = dat_003CF270;
    result->context = context;
    return result;
}
