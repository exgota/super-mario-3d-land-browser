namespace {
struct Sensor;
struct RelatedActor;
struct Nerve;

struct Actor {
    unsigned char unknown_00[0x64];
    RelatedActor* relatedActor;
    unsigned char unknown_68[0x48];
    volatile int floorTouchTimer;
    int pendingMessage;
    bool activated;
};
}

extern "C" bool fn_0026AB48(unsigned int);
extern "C" bool fn_00278710(unsigned int);
extern "C" bool fn_0026DBF0(int);
extern "C" bool fn_002786E4(unsigned int);
extern "C" RelatedActor* fn_002786AC(Sensor*, Sensor*);
extern "C" void fn_0027861C(RelatedActor*);
extern "C" bool _ZN2al21isMsgPlayerFloorTouchEj(unsigned int);
extern "C" bool _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(const Actor*, const Nerve*);
extern "C" void _ZN2al14onDrawClippingEPNS_9LiveActorE(Actor*);
extern "C" void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Actor*, const Nerve*);
extern "C" const Nerve dat_003F15C0;
extern "C" const Nerve dat_003F15C4;

namespace {
bool isTimedMessage(unsigned int message, int timer) {
    return timer > 0 && fn_00278710(message);
}
}

extern "C" bool fn_0014A878(Actor* actor, unsigned int message, Sensor* sender, Sensor* receiver) {
    if (fn_0026AB48(message))
        return true;

    if (_ZN2al21isMsgPlayerFloorTouchEj(message)) {
        actor->floorTouchTimer = 3;
        return true;
    }

    if (isTimedMessage(message, actor->floorTouchTimer))
        actor->pendingMessage = 1;

    if (isTimedMessage(message, actor->floorTouchTimer) &&
        fn_0026DBF0(0) &&
        _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F15C0))
        return true;

    if (fn_002786E4(message)) {
        actor->relatedActor = fn_002786AC(receiver, sender);
        fn_0027861C(actor->relatedActor);
        _ZN2al14onDrawClippingEPNS_9LiveActorE(actor);
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F15C4);
        actor->activated = true;
        return true;
    }
    return false;
}
