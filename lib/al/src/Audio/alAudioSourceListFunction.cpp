namespace {

struct AudioSourceFields;

// Only the observed function-pointer slot is typed.
struct AudioSourceMethodTable {
    void* mUnknownSlots[8];
    int (*mMethod0020)(AudioSourceFields*);
};

struct AudioSourceFields {
    AudioSourceMethodTable* mMethodTable;
};

struct AudioSourceListLink {
    AudioSourceListLink* mPrevious;
    AudioSourceListLink* mNext;
};

struct AudioSourceListNode {
    AudioSourceListLink mLink;
    AudioSourceFields* mSource;
    void* mOwner;
};

struct AudioSourceListFields {
    unsigned char mOpaquePrefix[0x20];
    AudioSourceListLink mHead;
};

}

extern "C" void fn_00244910(void* source, int parameter);

extern "C" void fn_001D9EC0(void* controller, int parameter) {
    AudioSourceListFields* fields = static_cast<AudioSourceListFields*>(controller);
    AudioSourceListLink* head = &fields->mHead;
    for (AudioSourceListLink* link = head->mNext; link != head; link = link->mNext) {
        AudioSourceListNode* node = reinterpret_cast<AudioSourceListNode*>(link);
        AudioSourceFields* source = node->mSource;
        if (source->mMethodTable->mMethod0020(source) == 0)
            fn_00244910(source, parameter);
    }
}
