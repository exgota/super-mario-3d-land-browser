namespace {
struct Object {
    unsigned char padding[0x74];
    float value;
};
}

extern "C" float fn_0032E980(const Object* self)
{
    return self->value;
}
