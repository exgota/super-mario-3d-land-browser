namespace {
class HitSensor;

struct SensorHitGroup {
    int mCapacity;
    int mSensorCount;
    HitSensor** mSensors;
};
}

extern "C" SensorHitGroup* fn_001CCEE4(SensorHitGroup* group, int capacity, const char*)
{
    group->mCapacity = capacity;
    group->mSensorCount = 0;
    group->mSensors = 0;
    group->mSensors = new HitSensor*[group->mCapacity];
    for (int i = 0; i < group->mCapacity; ++i)
        group->mSensors[i] = 0;
    return group;
}

extern "C" SensorHitGroup* fn_00243BA8(SensorHitGroup* group, int capacity, const char*)
{
    group->mCapacity = capacity;
    group->mSensorCount = 0;
    group->mSensors = 0;
    group->mSensors = new HitSensor*[group->mCapacity];
    for (int i = 0; i < group->mCapacity; ++i)
        group->mSensors[i] = 0;
    return group;
}
