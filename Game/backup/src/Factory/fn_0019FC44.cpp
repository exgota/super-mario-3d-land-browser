namespace {
struct Nerve { void* vtable; };
struct Sensor;
struct Message;
struct Actor {
    char reserved[0x64];
    Actor* partner;
};

extern "C" {
extern Nerve dat_003F3084;
extern Nerve dat_003F3088;
extern Nerve dat_003F308C;
bool _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(const Actor*, const Nerve*);
bool _ZN2al12isSensorNameEPNS_9HitSensorEPKc(Sensor*, const char*);
bool fn_0027B180(const Message*);
bool fn_0027A538(const Message*, Sensor*, Sensor*, Actor*, const Nerve*);
bool fn_0027B768(const Message*, Sensor*, Sensor*, Actor*, const Nerve*);
}
}

extern "C" bool fn_0019FC44(Actor* actor, const Message* message, Sensor* sender, Sensor* receiver) {
    if (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F3084)
        || _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F308C)
        || _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F3088))
        return false;
    if (!_ZN2al12isSensorNameEPNS_9HitSensorEPKc(receiver, "Body"))
        return false;
    const Nerve* nerve = fn_0027B180(message) ? &dat_003F3088 : &dat_003F3084;
    if (fn_0027A538(message, sender, receiver, actor, nerve))
        return true;
    if (fn_0027B768(message, sender, receiver, actor->partner, &dat_003F308C))
        return true;
    return false;
}
