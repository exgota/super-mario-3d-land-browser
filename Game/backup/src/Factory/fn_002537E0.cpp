namespace {
struct Target {
    unsigned char pad_00_17[0x18];
    float field_18;
};
struct Owner {
    unsigned char pad_00_1f[0x20];
    Target* field_20;
};
}

extern "C" void fn_002537E0(Owner* self, float value) {
    self->field_20->field_18 = value;
}
