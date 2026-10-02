namespace {
struct Actor {
    void **vtable;
};

struct Player {
    unsigned char pad[0x98];
    Actor *actor;
};

}

namespace rp { Player *getPlayerActor(); }
extern "C" int fn_00217584() {
    Actor *actor = rp::getPlayerActor()->actor;
    return reinterpret_cast<int (*)(Actor *)>(actor->vtable[0])(actor);
}

extern "C" int fn_002786F4() {
    Actor *actor = rp::getPlayerActor()->actor;
    return reinterpret_cast<int (*)(Actor *)>(actor->vtable[0])(actor);
}
