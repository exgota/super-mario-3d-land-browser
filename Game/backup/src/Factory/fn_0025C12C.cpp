namespace {
typedef unsigned int u32;
}

extern "C" u32 fn_0025C134(u32);

extern "C" u32 fn_0025C12C(void* object) {
    return fn_0025C134(*reinterpret_cast<u32*>(static_cast<char*>(object) + 0xe8));
}
