namespace {

// Partial retail layout. Concrete source and pointee identities remain unknown.
struct AudioSourceStateFields {
    unsigned char mOpaquePrefix[0x24];
    void* mInterface0024;
    unsigned char mOpaqueMiddle[0x1C];
    void* mObject0044;
};

}

extern "C" void* fn_00330618(void* source) {
    AudioSourceStateFields* fields = static_cast<AudioSourceStateFields*>(source);
    return fields->mObject0044;
}

extern "C" bool fn_00330664(void* source) {
    AudioSourceStateFields* fields = static_cast<AudioSourceStateFields*>(source);
    return fields->mInterface0024 == 0;
}
