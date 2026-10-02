extern "C" void* fn_001814B4(void*);

extern "C" void* fn_001814AC(void* self) {
    return fn_001814B4(*reinterpret_cast<void**>(
        reinterpret_cast<char*>(self) + 0x48));
}
