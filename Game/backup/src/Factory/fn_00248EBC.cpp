namespace {
struct Target {
    unsigned char padding[0x2e];
    unsigned char value;
};

struct Owner {
    unsigned char padding[8];
    Target* target;
};
}

extern "C" void fn_00248EBC(Owner* self, unsigned char value)
{
    self->target->value = value;
}
