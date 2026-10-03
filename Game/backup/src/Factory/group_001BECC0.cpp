namespace
{
struct CounterObject
{
    unsigned char padding[0x24];
    int counter;
};
}

extern "C" void fn_001BECC0(CounterObject *self)
{
    ++self->counter;
}

extern "C" void fn_001E07C8(CounterObject *self)
{
    ++self->counter;
}
