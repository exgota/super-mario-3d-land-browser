namespace {
struct Object {
    unsigned char padding[0x11];
    unsigned char value;
};
}

extern "C" unsigned char fn_0036DFD0(const Object* self)
{
    return self->value;
}
