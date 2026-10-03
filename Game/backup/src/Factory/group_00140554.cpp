namespace {
struct FlagObject {
    unsigned char padding[0xC];
    unsigned char flag;
};
}

extern "C" void fn_00140554(FlagObject *self) {
    self->flag = 1;
}

extern "C" void fn_001B4BF4(FlagObject *self) {
    self->flag = 1;
}
