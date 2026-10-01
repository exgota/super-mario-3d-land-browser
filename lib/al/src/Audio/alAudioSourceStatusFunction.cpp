namespace {

struct AudioSourceStatusFields;
struct AudioInterfaceStatusFields;

// Only independently observed method slots are typed.
struct AudioSourceStatusMethodTable {
    void* mUnknownSlots0000[8];
    int (*mMethod0020)(AudioSourceStatusFields*);
    void* mUnknownSlots0024[10];
    int (*mMethod004C)(AudioSourceStatusFields*);
};

struct AudioInterfaceStatusMethodTable {
    void* mUnknownSlots0000[2];
    int (*mMethod0008)(AudioInterfaceStatusFields*);
};

struct AudioInterfaceStatusFields {
    AudioInterfaceStatusMethodTable* mMethodTable;
};

struct AudioSourceStatusFields {
    AudioSourceStatusMethodTable* mMethodTable;
    unsigned char mOpaquePrefix0004[0x1C];
    AudioInterfaceStatusFields* mInterface0020;
    unsigned char mOpaqueMiddle0024[0x14];
    unsigned char mState0038;
    unsigned char mOpaqueMiddle0039[3];
    int mInteger003C;
};

}

extern "C" bool fn_00330678(void* source) {
    AudioSourceStatusFields* fields = static_cast<AudioSourceStatusFields*>(source);
    if (fields->mMethodTable->mMethod0020(fields) != 0)
        return true;
    if (fields->mMethodTable->mMethod004C(fields) != 0) {
        if (fields->mInteger003C <= 0)
            return true;
    } else if (fields->mState0038 != 0) {
        AudioInterfaceStatusFields* interface = fields->mInterface0020;
        if (interface->mMethodTable->mMethod0008(interface) == 0)
            return true;
    }
    return false;
}
