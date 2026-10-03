namespace {
struct InnerView {
    unsigned char pad[8];
    unsigned short value;
};
struct OuterView {
    unsigned char pad[0xC];
    InnerView *inner;
};
}

extern "C" unsigned short fn_00249CD8(OuterView *self) {
    return self->inner->value;
}
