namespace {
typedef int (*VirtualFunction)(void*, void*);
}

extern "C" int fn_00225234(void*, void*);

extern "C" int fn_00225224(void* self, void* argument) {
    void* object = *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(self) + 0x20);
    void** table = *reinterpret_cast<void***>(object);
    VirtualFunction function = reinterpret_cast<VirtualFunction>(table[2]);
    return function(object, argument);
}
