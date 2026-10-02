namespace {
struct ResettableObject {
    unsigned int vtable;
    unsigned char state;
};
}

extern "C" void fn_0019FB34(ResettableObject* self) { self->state = 0; }
extern "C" void fn_001A8260(ResettableObject* self) { self->state = 0; }
extern "C" void fn_001B99EC(ResettableObject* self) { self->state = 0; }
extern "C" void fn_001BB3F8(ResettableObject* self) { self->state = 0; }
extern "C" void fn_0021DCEC(ResettableObject* self) { self->state = 0; }
extern "C" void fn_002D5424(ResettableObject* self) { self->state = 0; }
extern "C" void fn_00376248(ResettableObject* self) { self->state = 0; }
