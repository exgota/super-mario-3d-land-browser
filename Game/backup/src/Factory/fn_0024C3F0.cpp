namespace al {
struct LiveActorKit;
LiveActorKit* getLiveActorKit();
}

extern "C" void* fn_0024C3F0() {
    return *reinterpret_cast<void**>(reinterpret_cast<char*>(al::getLiveActorKit()) + 0x18);
}
