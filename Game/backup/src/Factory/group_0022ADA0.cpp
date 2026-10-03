namespace {
struct ObjectFields {
    unsigned char pad[0x38];
    unsigned int field_38;
    unsigned int field_3C;
    unsigned int field_40;
};
}

extern "C" void fn_0022ADA0(ObjectFields* self) {
    self->field_38 = 0;
    self->field_3C = 0;
    self->field_40 = 0;
}

extern "C" void fn_002C16C0(ObjectFields* self) {
    self->field_38 = 0;
    self->field_3C = 0;
    self->field_40 = 0;
}
