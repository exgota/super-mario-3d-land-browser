namespace {
struct LiveActor;
struct Keeper {
    void request(LiveActor *, int);
};
}

namespace al {
struct LiveActor;
struct ExecuteRequestKeeper {
    void request(LiveActor *, int);
};
}

extern "C" void fn_00252B04(void *arg) {
    al::ExecuteRequestKeeper *keeper =
        *reinterpret_cast<al::ExecuteRequestKeeper **>(
            *reinterpret_cast<void **>(static_cast<char *>(arg) + 0x18));
    return keeper->request(reinterpret_cast<al::LiveActor *>(arg), 1);
}
