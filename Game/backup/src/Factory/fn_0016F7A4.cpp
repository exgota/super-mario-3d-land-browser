namespace {
struct Nerve;
struct SensorMsg;
struct HitSensor;
struct IUseNerve;
struct Actor {
    char padding[0x6c];
    Actor* relatedActor;
};
}

extern "C" {
extern Nerve dat_003F2244;
extern Nerve dat_003F2248;
extern Nerve dat_003F224C;
bool _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(const IUseNerve*, const Nerve*);
bool _ZN2al12isSensorNameEPNS_9HitSensorEPKc(HitSensor*, const char*);
bool fn_00271500(Actor*, const SensorMsg*, HitSensor*, HitSensor*, float);
bool fn_0027B180(const SensorMsg*);
bool fn_0027A538(const SensorMsg*, HitSensor*, HitSensor*, Actor*, const Nerve*);
bool fn_0027B768(const SensorMsg*, HitSensor*, HitSensor*, Actor*, const Nerve*);

bool fn_0016F7A4(Actor* actor, const SensorMsg* message, HitSensor* sender, HitSensor* receiver) {
    return !_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(actor), &dat_003F2244)
        && !_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(actor), &dat_003F224C)
        && !_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(actor), &dat_003F2248)
        && _ZN2al12isSensorNameEPNS_9HitSensorEPKc(receiver, "Body")
        && (fn_00271500(actor, message, sender, receiver, 0.01f)
            || fn_0027A538(message, sender, receiver, actor,
                fn_0027B180(message) ? &dat_003F2248 : &dat_003F2244)
            || fn_0027B768(message, sender, receiver, actor->relatedActor, &dat_003F224C));
}
}
