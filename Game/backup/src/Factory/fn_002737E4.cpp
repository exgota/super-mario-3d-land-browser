namespace {
extern "C" int fn_0026AA60(int);
extern "C" int fn_00273800(int);
}

extern "C" int fn_002737E4(void *arg) {
    int value = *reinterpret_cast<int *>(*reinterpret_cast<char **>(static_cast<char *>(arg) + 0x64) + 0x10);
    if (value != 0)
        value = fn_0026AA60(value);
    return fn_00273800(value);
}
