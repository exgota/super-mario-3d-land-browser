namespace {
struct ResultHolder {
    unsigned int padding;
    unsigned int value;
};

struct AccessorObject {
    unsigned int padding[0x68 / 4];
    ResultHolder *holder;
};
}

extern "C" unsigned int fn_00141CD4(AccessorObject *object) {
    return object->holder->value;
}
