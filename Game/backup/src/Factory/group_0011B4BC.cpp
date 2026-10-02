namespace {
struct ActorStorage {
    unsigned char padding[0x80];
    void* resource;
};

extern "C" void fn_0027ADD4(void* resource);
}

namespace al {
class LiveActor {
public:
    void kill();
};
}

extern "C" void fn_0011B4BC(ActorStorage* self) {
    fn_0027ADD4(self->resource);
    reinterpret_cast<al::LiveActor*>(self)->kill();
}

extern "C" void fn_0013FBC4(ActorStorage* self) {
    fn_0027ADD4(self->resource);
    reinterpret_cast<al::LiveActor*>(self)->kill();
}
