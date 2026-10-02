namespace {
struct Element {
    unsigned char bytes[92];
};

struct Owner {
    unsigned char padding[0x20];
    Element *elements;
};
}

extern "C" Element *fn_00216EB0(Owner *owner, int index) {
    return &owner->elements[index];
}
