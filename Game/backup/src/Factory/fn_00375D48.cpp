namespace al
{
    struct LiveActor;
    extern const void* getTrans(const LiveActor*);
}

extern "C" const void* fn_00375D48(const void* self)
{
    return al::getTrans(reinterpret_cast<const al::LiveActor*>(
        static_cast<const char*>(self) - 0x64));
}
