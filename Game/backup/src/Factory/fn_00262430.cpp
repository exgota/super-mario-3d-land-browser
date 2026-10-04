namespace {
struct ValueHolder {
    unsigned char padding[0x40];
    float value;
};

struct Owner {
    unsigned char padding[0x10];
    ValueHolder* holder;
};
}

extern "C" float fn_00262430(Owner* self) {
    return self->holder->value;
}
