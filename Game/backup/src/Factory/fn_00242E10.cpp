extern "C" void* fn_00242E1C(void*);

extern "C" void* fn_00242E10(void* self)
{
    void* value = *reinterpret_cast<void**>(reinterpret_cast<char*>(self) + 0x20);
    return fn_00242E1C(reinterpret_cast<char*>(value) + 0x150);
}
