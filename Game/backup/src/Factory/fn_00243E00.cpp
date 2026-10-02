namespace
{
struct SensorHitGroupLayout
{
    int mCapacity;
    int mSensorCount;
    void** mSensors;
};
}

extern "C" void fn_00243E00(SensorHitGroupLayout* group, void* sensor)
{
    group->mSensors[group->mSensorCount] = sensor;
    ++group->mSensorCount;
}
