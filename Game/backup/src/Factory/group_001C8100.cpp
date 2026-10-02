namespace {
typedef unsigned int u32;
}

extern "C" u32 fn_002D5340(u32, u32);
extern "C" u32 fn_00250EE0(u32, u32);

extern "C" u32 fn_001C8100(u32 value) {
    return fn_002D5340(value, 0x10);
}

extern "C" u32 fn_002734B0(u32 value) {
    return fn_00250EE0(value, 0x10);
}
