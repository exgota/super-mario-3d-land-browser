namespace {
struct PendingState {
    unsigned char reserved00[0x88];
    int value;
    int pending;
    unsigned char reserved90[4];
    bool latched;
};
}

extern "C" void fn_00122CC4(PendingState* self, int value) {
    if (!self->latched) {
        self->value = value;
        self->pending = 1;
        self->latched = true;
    }
}
