namespace {
struct CheckTarget {
    unsigned char pad[8];
    int value;
};
}

extern "C" int fn_002BD928(CheckTarget *self) {
    return self->value != 0;
}

extern "C" int fn_003351AC(CheckTarget *self) {
    return self->value != 0;
}
