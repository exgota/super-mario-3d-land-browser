namespace {
typedef int (*Callback)(void*, int, int);
}

extern "C" int fn_001CC4E0(void* self, int a, int b) {
    return reinterpret_cast<Callback>(*reinterpret_cast<Callback*>(
        static_cast<unsigned char*>(self) + 0x20))(self, a, b);
}
