extern "C" int fn_00242D28(int);

extern "C" int fn_00242D20(void* self) {
    return fn_00242D28(*reinterpret_cast<int*>(static_cast<char*>(self) + 0x1f0));
}
