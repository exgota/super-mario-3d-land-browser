namespace {
struct FlagObject {
    char padding[0x5C];
    unsigned char flag;
};
}

extern "C" void fn_001BB688(FlagObject *self) {
    self->flag = 1;
}

extern "C" void fn_001DEC68(FlagObject *self) {
    self->flag = 1;
}
