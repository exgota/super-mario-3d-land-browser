namespace {

// Byte storage is independently observed. The target returns signed extension.
struct AudioSourceFlagFields {
    unsigned char mOpaquePrefix[0x40];
    signed char mState0040;
};

}

extern "C" int fn_00330610(void* source) {
    AudioSourceFlagFields* fields = static_cast<AudioSourceFlagFields*>(source);
    return fields->mState0040;
}
