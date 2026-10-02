namespace {
struct IUseNerve;
struct Nerve;
struct HitSensor;
}

extern "C" {
extern Nerve dat_003F2FE8;
bool _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(const IUseNerve*, const Nerve*);
bool _ZN2al12isSensorNameEPNS_9HitSensorEPKc(HitSensor*, const char*);
bool _ZN2al9sendMsg41EPNS_9HitSensorES1_(HitSensor*, HitSensor*);
bool _ZN2al18sendMsgEnemyAttackEPNS_9HitSensorES1_(HitSensor*, HitSensor*);

void fn_001A6AD4(IUseNerve* actor, HitSensor* me, HitSensor* other)
{
    if (!_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2FE8)
        && _ZN2al12isSensorNameEPNS_9HitSensorEPKc(me, "Attack")) {
        _ZN2al9sendMsg41EPNS_9HitSensorES1_(other, me);
        _ZN2al18sendMsgEnemyAttackEPNS_9HitSensorES1_(other, me);
    }
}
}
