namespace {
typedef void* Ptr;
}

extern "C" int fn_001C2BC8(Ptr);
extern "C" int fn_001E1E18(Ptr);

extern "C" int fn_001C2BC0(Ptr self)
{
    return fn_001C2BC8(*reinterpret_cast<Ptr*>(reinterpret_cast<char*>(self) + 0x10));
}

extern "C" int fn_001E1E10(Ptr self)
{
    return fn_001E1E18(*reinterpret_cast<Ptr*>(reinterpret_cast<char*>(self) + 0x10));
}
