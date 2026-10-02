namespace {
struct NerveKeeper;
struct NerveExecutor {
    void* vtable;
    NerveKeeper* mNerveKeeper;
};
}

extern "C" void fn_002A8D2C(NerveKeeper*);
extern "C" void fn_002BD5A4(NerveKeeper*);
extern "C" void fn_00332958(NerveKeeper*);
extern "C" void fn_001DD958(NerveKeeper*);

extern "C" void fn_001BD908(NerveExecutor* self) {
    NerveKeeper* keeper = self->mNerveKeeper;
    if (keeper)
        fn_002A8D2C(keeper);
}

extern "C" void fn_001E1774(NerveExecutor* self) {
    NerveKeeper* keeper = self->mNerveKeeper;
    if (keeper)
        fn_002BD5A4(keeper);
}

extern "C" void fn_00276CF0(NerveExecutor* self) {
    NerveKeeper* keeper = self->mNerveKeeper;
    if (keeper)
        fn_00332958(keeper);
}

extern "C" void fn_00277660(NerveExecutor* self) {
    NerveKeeper* keeper = self->mNerveKeeper;
    if (keeper)
        fn_001DD958(keeper);
}
