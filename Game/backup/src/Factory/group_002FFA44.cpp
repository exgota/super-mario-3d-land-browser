namespace {
struct Object {
    const unsigned char* volatile tables[2];
    volatile bool flag;
};

extern "C" const unsigned char dat_003DADC0[];
extern "C" const unsigned char dat_003DAE28[];
}

extern "C" void fn_002FFA44(Object* self) {
    const unsigned char* table = dat_003DADC0;
    for (unsigned i = 0; i < 2; ++i, table += 0x20)
        self->tables[i] = table;
    self->flag = false;
}

extern "C" void fn_002FFBF4(Object* self) {
    const unsigned char* table = dat_003DAE28;
    for (unsigned i = 0; i < 2; ++i, table += 0x20)
        self->tables[i] = table;
    self->flag = false;
}
