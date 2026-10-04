namespace {
extern "C" int fn_002737C0(void *, int);
}

extern "C" int fn_002737B4(void *arg) {
    void *value = *reinterpret_cast<void **>(static_cast<char *>(arg) + 0x10);
    return fn_002737C0(value, -1);
}
