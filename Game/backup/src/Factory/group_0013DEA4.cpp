namespace {
struct StoreAt30 {
    unsigned char padding[0x30];
    unsigned int value;
};
}

extern "C" void fn_0013DEA4(StoreAt30 *object, unsigned int value) {
    object->value = value;
}

extern "C" void fn_002E00F0(StoreAt30 *object, unsigned int value) {
    object->value = value;
}
