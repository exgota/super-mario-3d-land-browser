namespace al {
struct HitSensor;
void sendMsg41(HitSensor*, HitSensor*);
void sendMsgEnemyAttack(HitSensor*, HitSensor*);
}

extern "C" void fn_0027D540(void*, al::HitSensor* arg1, al::HitSensor* arg2) {
    al::sendMsg41(arg2, arg1);
}

extern "C" void fn_0027D564(void*, al::HitSensor* arg1, al::HitSensor* arg2) {
    al::sendMsgEnemyAttack(arg2, arg1);
}
