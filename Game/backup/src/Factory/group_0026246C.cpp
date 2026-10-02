namespace {
struct Input {
    unsigned char pad_00[4];
    void* field_04;
};
struct Intermediate {
    unsigned char pad_00[4];
    struct Result* field_04;
};
struct Result {
    unsigned char pad_00[0x14];
    float value_14;
};
}

extern "C" Intermediate* fn_0024E5E0(void*);

extern "C" float fn_0026246C(Input* self) {
    return fn_0024E5E0(self->field_04)->field_04->value_14;
}
extern "C" float fn_002624AC(Input* self) {
    return fn_0024E5E0(self->field_04)->field_04->value_14;
}
extern "C" float fn_002624EC(Input* self) {
    return fn_0024E5E0(self->field_04)->field_04->value_14;
}
