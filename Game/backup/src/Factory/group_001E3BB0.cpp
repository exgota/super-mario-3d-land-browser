namespace
{
struct GroupBody
{
    unsigned char pad[0xC];
    unsigned int count;
    void** actors;
};
}

extern "C" void fn_001E3BB0(GroupBody* self, void* actor)
{
    self->actors[self->count] = actor;
    ++self->count;
}

extern "C" void fn_001E5464(GroupBody* self, void* actor)
{
    self->actors[self->count] = actor;
    ++self->count;
}

extern "C" void fn_001E5F0C(GroupBody* self, void* actor)
{
    self->actors[self->count] = actor;
    ++self->count;
}
