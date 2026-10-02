namespace
{
struct StateFlagLayout
{
    unsigned char padding[8];
    bool isDead;
};
}

extern "C" void fn_00141418(StateFlagLayout* self)
{
    self->isDead = true;
}

extern "C" void fn_00143028(StateFlagLayout* self)
{
    self->isDead = true;
}

extern "C" void fn_0019597C(StateFlagLayout* self)
{
    self->isDead = true;
}

extern "C" void fn_001A82B0(StateFlagLayout* self)
{
    self->isDead = true;
}

extern "C" void fn_001A871C(StateFlagLayout* self)
{
    self->isDead = true;
}

extern "C" void fn_001B7FD8(StateFlagLayout* self)
{
    self->isDead = true;
}

extern "C" void fn_00376848(StateFlagLayout* self)
{
    self->isDead = true;
}
