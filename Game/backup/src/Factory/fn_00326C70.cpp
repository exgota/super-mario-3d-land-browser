namespace {
struct UnknownObject {
    char pad[0x3b];
    signed char value;
};
}

extern "C" signed char fn_00326C70(UnknownObject *self)
{
    return self->value;
}
