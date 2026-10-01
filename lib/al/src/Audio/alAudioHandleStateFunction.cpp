namespace {

struct AudioHandleStateFields {
    void* mObject;
};

}

extern "C" bool fn_0023F93C(void* handle) {
    AudioHandleStateFields* fields = static_cast<AudioHandleStateFields*>(handle);
    return fields->mObject != 0;
}
