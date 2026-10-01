namespace {

// Partial retail layout. Concrete field identities remain unknown.
struct AudioSourceFieldView {
    unsigned char mOpaquePrefix[0x20];
    void* mInterface0020;
    unsigned char mOpaqueMiddle[0x10];
    void* mObject0034;
    signed char mState0038;
};

}

extern "C" void* fn_00330620(void* source) {
    AudioSourceFieldView* fields = static_cast<AudioSourceFieldView*>(source);
    return fields->mObject0034;
}

extern "C" void* fn_00330628(void* source) {
    AudioSourceFieldView* fields = static_cast<AudioSourceFieldView*>(source);
    return fields->mInterface0020;
}

extern "C" int fn_003306F4(void* source) {
    AudioSourceFieldView* fields = static_cast<AudioSourceFieldView*>(source);
    return fields->mState0038;
}
