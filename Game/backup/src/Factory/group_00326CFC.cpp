namespace {
struct ByteFieldAtC {
    char padding[0xC];
    signed char value;
};
}

extern "C" signed char fn_00326CFC(const ByteFieldAtC* self) { return self->value; }
extern "C" signed char fn_0032B708(const ByteFieldAtC* self) { return self->value; }
extern "C" signed char fn_0032E758(const ByteFieldAtC* self) { return self->value; }
extern "C" signed char fn_00376424(const ByteFieldAtC* self) { return self->value; }
