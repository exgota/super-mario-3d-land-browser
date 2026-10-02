namespace {
struct TimerState {
    unsigned char pad[0x64];
    int counter;
};
}

extern "C" void fn_0013EFE0(TimerState* self) {
    if (self->counter > 0)
        --self->counter;
}

extern "C" void fn_001572A0(TimerState* self) {
    if (self->counter > 0)
        --self->counter;
}

extern "C" void fn_00157778(TimerState* self) {
    if (self->counter > 0)
        --self->counter;
}
