namespace {

struct AudioInterfaceFields;

struct AudioInterfaceMethodTable {
    void* mUnknownSlots[2];
    int (*mMethod0008)(AudioInterfaceFields*);
};

struct AudioInterfaceFields {
    AudioInterfaceMethodTable* mMethodTable;
};

struct AudioSourceInterfaceFields {
    unsigned char mOpaquePrefix[0x20];
    AudioInterfaceFields* mInterface0020;
};

}

extern "C" int fn_00330630(void* source) {
    AudioSourceInterfaceFields* fields = static_cast<AudioSourceInterfaceFields*>(source);
    AudioInterfaceFields* interface = fields->mInterface0020;
    return interface->mMethodTable->mMethod0008(interface);
}
