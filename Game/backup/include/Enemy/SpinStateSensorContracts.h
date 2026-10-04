#pragma once

namespace al
{
class HitSensor;
class IUseNerve;
class Nerve;
}

// The receiver is the spin state; its host LiveActor is stored at state + 0x0C.
extern "C" void fn_0026CD08( al::IUseNerve* state, al::HitSensor* own, al::HitSensor* other );
extern "C" bool fn_0032DA20( al::IUseNerve* state, al::HitSensor* own, al::HitSensor* other );
// Retain the current sensor-family return declaration; this caller ignores it.
extern "C" bool fn_0027A4EC( al::HitSensor* to, al::HitSensor* from );
// Existing singleton: table 0x003C0494, execute entry 0x0036C334.
extern "C" const al::Nerve dat_003F2E90;
