namespace {
struct UnknownObject {
    unsigned char padding[0x8A];
    unsigned char field_8A;
};
}

extern "C" void fn_00128D7C(UnknownObject* self)
{
    self->field_8A = 1;
}
