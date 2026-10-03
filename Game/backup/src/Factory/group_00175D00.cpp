namespace {
struct Nerve {};
struct HitSensor {};
struct SensorMessage {};
struct IUseNerve {
    char unknown[0x64];
    void* field_64;
};

extern "C" {
bool _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(const IUseNerve*, const Nerve*);
bool _ZN2al12isSensorNameEPNS_9HitSensorEPKc(HitSensor*, const char*);
bool fn_0027B180(const SensorMessage*);
bool fn_0027A538(const SensorMessage*, HitSensor*, HitSensor*, IUseNerve*, const Nerve*);
bool fn_0027B768(const SensorMessage*, HitSensor*, HitSensor*, void*, const Nerve*);
extern Nerve dat_003F2364;
extern Nerve dat_003F2360;
extern Nerve dat_003F235C;
extern Nerve dat_003F2338;
extern Nerve dat_003F2334;
extern Nerve dat_003F2330;
}
}

extern "C" bool fn_00175D00(IUseNerve* actor, const SensorMessage* message,
                             HitSensor* sender, HitSensor* receiver) {
    if (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2364) ||
        _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2360) ||
        _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F235C))
        return false;
    if (!_ZN2al12isSensorNameEPNS_9HitSensorEPKc(receiver, "Body"))
        return false;
    const Nerve* nerve = fn_0027B180(message) ? &dat_003F2360 : &dat_003F235C;
    if (fn_0027A538(message, sender, receiver, actor, nerve))
        return true;
    if (fn_0027B768(message, sender, receiver, actor->field_64, &dat_003F2364))
        return true;
    return false;
}

extern "C" bool fn_00318244(IUseNerve* actor, const SensorMessage* message,
                             HitSensor* sender, HitSensor* receiver) {
    if (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2338) ||
        _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2334) ||
        _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2330))
        return false;
    if (!_ZN2al12isSensorNameEPNS_9HitSensorEPKc(receiver, "Body"))
        return false;
    const Nerve* nerve = fn_0027B180(message) ? &dat_003F2334 : &dat_003F2330;
    if (fn_0027A538(message, sender, receiver, actor, nerve))
        return true;
    if (fn_0027B768(message, sender, receiver, actor->field_64, &dat_003F2338))
        return true;
    return false;
}
