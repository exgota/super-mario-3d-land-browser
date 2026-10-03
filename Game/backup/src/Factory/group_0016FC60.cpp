namespace {
struct StoreAt74 {
    unsigned char padding[0x74];
    unsigned int value;
};
}

extern "C" void fn_0016FC60(StoreAt74* self, unsigned int value) {
    self->value = value;
}

extern "C" void fn_001B6910(StoreAt74* self, unsigned int value) {
    self->value = value;
}
