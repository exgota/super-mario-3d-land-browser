namespace {
struct OwnerView {
    unsigned int reserved[9];
    unsigned int field_24;
};
}

extern "C" unsigned int fn_002271E8(unsigned int, unsigned int, unsigned int);

extern "C" unsigned int fn_002CD200(unsigned int a0, unsigned int a1, OwnerView *a2) {
    return fn_002271E8(a0, a1, a2->field_24);
}
