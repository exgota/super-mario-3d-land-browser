namespace {
struct Subject {
    unsigned int unknown_00;
    void* unknown_04;
};
}

extern "C" void* fn_00216E3C(void* value);

extern "C" int fn_00262484(Subject* self) {
    return fn_00216E3C(self->unknown_04) != 0;
}

extern "C" int fn_002624C4(Subject* self) {
    return fn_00216E3C(self->unknown_04) != 0;
}

extern "C" int fn_00262504(Subject* self) {
    return fn_00216E3C(self->unknown_04) != 0;
}
