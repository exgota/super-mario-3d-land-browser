extern "C" void* fn_002913CC();
extern "C" void* fn_0025BB50(void*);
extern "C" void* fn_0025B974(void*, void*);
extern "C" void* fn_0016D134(void*);

extern "C" void* fn_0016D114(void* value) {
    void* context = fn_002913CC();
    void* receiver = fn_0025BB50(context);
    return fn_0016D134(fn_0025B974(receiver, value));
}
