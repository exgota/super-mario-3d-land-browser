namespace
{
struct Fn00141CE0Object
{
    unsigned char pad[0x68];
    void **value;
};
}

extern "C" void *fn_00141CE0(Fn00141CE0Object *self)
{
    return *self->value;
}
