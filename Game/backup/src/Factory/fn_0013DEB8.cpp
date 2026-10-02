namespace {
struct LiveActor {
    unsigned char pad[0x38];
    void* value;
};
}

extern "C" void* fn_0013DEB8(LiveActor* actor) {
    return actor->value;
}
