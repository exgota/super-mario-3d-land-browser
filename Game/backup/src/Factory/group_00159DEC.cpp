extern "C" void fn_001BFA04(void*, const char*);
extern "C" void fn_002796C0(void*, const char*);
extern "C" void _ZN2al9LiveActor12startClippedEv(void*);
extern "C" void _ZN2al9LiveActor4killEv(void*);

extern "C" void fn_00159DEC(void* self)
{
    fn_001BFA04(static_cast<char*>(self) + 4, "Wait");
    _ZN2al9LiveActor12startClippedEv(self);
}

extern "C" void fn_0015AFC4(void* self)
{
    fn_001BFA04(static_cast<char*>(self) + 4, "Wait");
    _ZN2al9LiveActor12startClippedEv(self);
}

extern "C" void fn_0015B0D8(void* self)
{
    fn_002796C0(static_cast<char*>(self) + 4, "Wait");
    _ZN2al9LiveActor4killEv(self);
}
