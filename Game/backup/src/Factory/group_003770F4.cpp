namespace {
typedef void (*ThunkFn)(void*);

void dispatch_003770F4(void* object) {
    void* adjusted = static_cast<char*>(object) - 0x60;
    void** table = *static_cast<void***>(adjusted);
    reinterpret_cast<ThunkFn>(table[6])(adjusted);
}

void dispatch_003774B4(void* object) {
    void* adjusted = static_cast<char*>(object) - 0x60;
    void** table = *static_cast<void***>(adjusted);
    reinterpret_cast<ThunkFn>(table[6])(adjusted);
}
}

extern "C" void fn_003770F4(void* object) { dispatch_003770F4(object); }
extern "C" void fn_003774B4(void* object) { dispatch_003774B4(object); }
