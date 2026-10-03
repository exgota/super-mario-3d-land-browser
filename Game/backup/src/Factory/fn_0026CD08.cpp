namespace {
struct HitSensor;
struct IUseNerve;
struct Nerve;
}

extern "C" {
bool _ZN2al12isSensorNameEPNS_9HitSensorEPKc(HitSensor*, const char*);
bool _ZN2al14isSensorPlayerEPKNS_9HitSensorE(const HitSensor*);
bool _ZN2al14isSensorMapObjEPKNS_9HitSensorE(const HitSensor*);
bool _ZN2al9sendMsg41EPNS_9HitSensorES1_(HitSensor*, HitSensor*);
bool _ZN2al18sendMsgEnemyAttackEPNS_9HitSensorES1_(HitSensor*, HitSensor*);
bool _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(const IUseNerve*, const Nerve*);
bool fn_0032DA20(IUseNerve*, HitSensor*, HitSensor*);
bool fn_0027A4EC(HitSensor*, HitSensor*);
extern const Nerve dat_003F2E90;
}

extern "C" void fn_0026CD08(IUseNerve* actor, HitSensor* own, HitSensor* other)
{
    if (_ZN2al12isSensorNameEPNS_9HitSensorEPKc(own, "SpinArm") &&
        _ZN2al14isSensorPlayerEPKNS_9HitSensorE(other)) {
        if (fn_0032DA20(actor, own, other)) {
            _ZN2al9sendMsg41EPNS_9HitSensorES1_(other, own);
            if (!_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2E90))
                _ZN2al18sendMsgEnemyAttackEPNS_9HitSensorES1_(other, own);
        }
    } else if (_ZN2al12isSensorNameEPNS_9HitSensorEPKc(own, "Block") &&
               _ZN2al14isSensorMapObjEPKNS_9HitSensorE(other) &&
               fn_0032DA20(actor, own, other)) {
        fn_0027A4EC(other, own);
    }
}
