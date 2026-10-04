namespace {
struct Fn0024F1DCObject {
    unsigned char pad_00[0x24];
    void* field_24;
    unsigned char pad_28[4];
    int field_2C;
};

extern "C" int fn_0024F1E8(void*, int);
}

extern "C" int fn_0024F1DC(Fn0024F1DCObject* self) {
    return fn_0024F1E8(self->field_24, self->field_2C);
}
