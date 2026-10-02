namespace {
struct State {
    unsigned int reserved;
    void* result;
    unsigned int padding[6];
    void* ready;
};
}

extern "C" State* fn_0025BB60();
extern "C" void* fn_002913CC();
extern "C" void* fn_0025BB50(void*);
extern "C" void* fn_0025B7CC(void*);

extern "C" void* fn_0025B78C() {
    if (fn_0025BB60()->ready)
        return fn_0025BB60()->result;
    return fn_0025B7CC(fn_0025BB50(fn_002913CC()));
}
