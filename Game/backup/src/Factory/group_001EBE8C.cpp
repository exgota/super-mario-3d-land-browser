namespace {
struct StoreTarget {
    unsigned char padding[0x50];
    int value;
};
}

extern "C" void fn_001EBE8C(StoreTarget *self, int value) {
    self->value = value;
}

extern "C" void fn_003761B4(StoreTarget *self, int value) {
    self->value = value;
}
