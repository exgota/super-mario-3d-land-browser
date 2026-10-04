extern "C" int fn_002734F4(void*, int);

extern "C" int fn_002734E8(void* self)
{
    void* object = *reinterpret_cast<void**>(reinterpret_cast<char*>(self) + 0x30);
    return fn_002734F4(object, 1);
}
