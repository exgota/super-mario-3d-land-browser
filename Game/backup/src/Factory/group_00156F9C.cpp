namespace {
struct Actor;
struct Sensor;
struct Controller;

struct ActorVTable {
    void* slots[25];
    bool (*isInactive)(Actor*);
};

struct Actor {
    ActorVTable* vtable;
    unsigned char padding[0x5c];
    Controller* controller;
    int cooldown;
};
}

namespace al {
extern bool isMsgPlayerFireBallAttack(unsigned int);
extern bool isMsgPlayerTailAttack(unsigned int);
extern bool isMsgKickKouraReflect(unsigned int);
extern bool isMsgKickStoneAttack(unsigned int);
extern bool isMsgPlayerBoomerangReflect(unsigned int);
extern bool isMsgPlayerInvincibleAttack(unsigned int);
}

extern "C" bool fn_0027D760(unsigned int, Sensor*, Sensor*);
extern "C" bool fn_002620C4(Sensor*, Sensor*);
extern "C" void fn_0027A624(Actor*, Sensor*, Sensor*);
extern "C" void fn_00192C08(Controller*);

extern "C" bool fn_00156F9C(Actor* actor, unsigned int message,
                            Sensor* sender, Sensor* receiver) {
    if (actor->vtable->isInactive(actor))
        return false;
    if (al::isMsgPlayerFireBallAttack(message)) {
        if (actor->cooldown == 0) {
            fn_0027D760(message, receiver, sender);
            actor->cooldown = 10;
        }
        fn_002620C4(sender, receiver);
        return false;
    }
    if (al::isMsgPlayerTailAttack(message) ||
        al::isMsgKickKouraReflect(message) ||
        al::isMsgKickStoneAttack(message) ||
        al::isMsgPlayerBoomerangReflect(message)) {
        fn_0027D760(message, sender, receiver);
        return true;
    }
    if (al::isMsgPlayerInvincibleAttack(message)) {
        fn_0027D760(message, receiver, sender);
        fn_0027A624(actor, sender, receiver);
        fn_00192C08(actor->controller);
        return true;
    }
    return false;
}

extern "C" bool fn_00157474(Actor* actor, unsigned int message,
                            Sensor* sender, Sensor* receiver) {
    if (actor->vtable->isInactive(actor))
        return false;
    if (al::isMsgPlayerFireBallAttack(message)) {
        if (actor->cooldown == 0) {
            fn_0027D760(message, receiver, sender);
            actor->cooldown = 10;
        }
        fn_002620C4(sender, receiver);
        return false;
    }
    if (al::isMsgPlayerTailAttack(message) ||
        al::isMsgKickKouraReflect(message) ||
        al::isMsgKickStoneAttack(message) ||
        al::isMsgPlayerBoomerangReflect(message)) {
        fn_0027D760(message, sender, receiver);
        return true;
    }
    if (al::isMsgPlayerInvincibleAttack(message)) {
        fn_0027D760(message, receiver, sender);
        fn_0027A624(actor, sender, receiver);
        fn_00192C08(actor->controller);
        return true;
    }
    return false;
}
