namespace {

// Partial byte view. Original flag meanings and declarations remain unknown.
struct AudioSourcePredicateFields {
    unsigned char mOpaquePrefix[0x38];
    unsigned char mState0038;
    unsigned char mOpaqueMiddle[0x0A];
    unsigned char mState0043;
};

}

extern "C" bool fn_00330640(void* source) {
    AudioSourcePredicateFields* fields = static_cast<AudioSourcePredicateFields*>(source);
    return fields->mState0038 == 0 && fields->mState0043 == 0;
}
