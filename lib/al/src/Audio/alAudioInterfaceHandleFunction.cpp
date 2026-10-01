namespace {

// Partial interface layout recovered from the independent constructor.
struct AudioInterfaceHandleFields {
    void* mMethodTable;
    void* mHandleObject;
    void* mSecondaryMethodTable;
};

}

extern "C" bool fn_0023F93C(void* handle);

extern "C" bool fn_00331704(void* interface) {
    AudioInterfaceHandleFields* fields = static_cast<AudioInterfaceHandleFields*>(interface);
    return fn_0023F93C(&fields->mHandleObject);
}
