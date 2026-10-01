namespace {

// Partial retail layout. The complete concrete class identity remains unknown.
struct AudioActionStateFields {
    void* mVirtualTable;
    const char* mActionName;
    const char* mActionGroupName;
};

}

extern "C" void fn_001D6580(void* state) {
    AudioActionStateFields* fields = static_cast<AudioActionStateFields*>(state);
    fields->mActionName = 0;
    fields->mActionGroupName = 0;
}
