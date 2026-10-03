namespace {
typedef void (*IconDrawDoneCallback)();

struct CFLiContext {
    unsigned char unknown[0xDC];
    IconDrawDoneCallback iconDrawDoneCallback;
};

extern "C" CFLiContext* fn_002881A4();
extern "C" void fn_001173A0();
}

extern "C" void CFLi_SetIconDrawDoneCallback(IconDrawDoneCallback callback) {
    CFLiContext* context = fn_002881A4();
    if (!callback)
        callback = fn_001173A0;
    context->iconDrawDoneCallback = callback;
}
