namespace {
struct AudioHandleStateFields {
    void* mObject;
};
}

extern "C" bool fn_0021D970(void* handle) {
    AudioHandleStateFields* fields = static_cast<AudioHandleStateFields*>(handle);
    return fields->mObject != 0;
}

extern "C" bool fn_00265C40(void* handle) {
    AudioHandleStateFields* fields = static_cast<AudioHandleStateFields*>(handle);
    return fields->mObject != 0;
}

extern "C" bool fn_00327AB0(void* handle) {
    AudioHandleStateFields* fields = static_cast<AudioHandleStateFields*>(handle);
    return fields->mObject != 0;
}
