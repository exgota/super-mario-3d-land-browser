namespace {
struct Context {
    unsigned char padding[0x24];
    int index;
};
}

extern "C" Context* fn_0025BB60();
extern "C" void* fn_002913CC();
extern "C" void* fn_0025BB50(void*);
extern "C" void fn_0025B6D4(void*, int);
extern "C" void fn_0025B8FC(void*, int);

extern "C" void fn_002179CC() {
    int index = fn_0025BB60()->index;
    fn_0025B6D4(fn_0025BB50(fn_002913CC()), index);
}

extern "C" void fn_0021E100() {
    int index = fn_0025BB60()->index;
    fn_0025B8FC(fn_0025BB50(fn_002913CC()), index);
}
