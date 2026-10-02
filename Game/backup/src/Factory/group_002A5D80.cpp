namespace {
struct Object {
    unsigned char pad[0x1c];
    void* field;
};
}

extern "C" void* fn_002A5D80(Object* self) { return self->field; }
extern "C" void* fn_002A782C(Object* self) { return self->field; }
extern "C" void* fn_002A7C48(Object* self) { return self->field; }
extern "C" void* fn_0033B43C(Object* self) { return self->field; }
extern "C" void* fn_0033B9C0(Object* self) { return self->field; }
extern "C" void* fn_0033C7C0(Object* self) { return self->field; }
extern "C" void* fn_0035D6C0(Object* self) { return self->field; }
extern "C" void* fn_003761AC(Object* self) { return self->field; }
