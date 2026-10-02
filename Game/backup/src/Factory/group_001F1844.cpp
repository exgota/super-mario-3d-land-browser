namespace {
struct ObjectWithFlag {
    unsigned char padding[4];
    int flag;
};
}

extern "C" int fn_001F1844(ObjectWithFlag *self) {
    return self->flag != 0;
}

extern "C" int fn_0022AC18(ObjectWithFlag *self) {
    return self->flag != 0;
}

extern "C" int fn_0032E3F8(ObjectWithFlag *self) {
    return self->flag != 0;
}
