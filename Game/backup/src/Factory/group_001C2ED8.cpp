namespace {
struct FinalNode {
    unsigned char padding[0x2C];
    void* value;
};

struct MiddleNode {
    FinalNode* next;
};

struct EntryObject {
    unsigned char padding[0x28];
    MiddleNode* member;
};
}

extern "C" void fn_0024EB8C(void*);
extern "C" void fn_0024947C(void*);
extern "C" void fn_002624AC(void*);
extern "C" void fn_00262430(void*);
extern "C" void fn_0024E9A8(void*);

extern "C" void fn_001C2ED8(EntryObject* self) {
    fn_0024EB8C(self->member->next->value);
}

extern "C" void fn_001D2C68(EntryObject* self) {
    fn_0024947C(self->member->next->value);
}

extern "C" void fn_001DB340(EntryObject* self) {
    fn_002624AC(self->member->next->value);
}

extern "C" void fn_001DD0B0(EntryObject* self) {
    fn_00262430(self->member->next->value);
}

extern "C" void fn_0026357C(EntryObject* self) {
    fn_0024E9A8(self->member->next->value);
}
