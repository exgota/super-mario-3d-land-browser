namespace {
extern "C" void* fn_00232390(void*, void*);
}

extern "C" void* fn_00232384(void* a, void* b) {
    void* adjusted_a = static_cast<char*>(a) + 0x1dc;
    void* adjusted_b = static_cast<char*>(b) + 4;
    return fn_00232390(adjusted_a, adjusted_b);
}
