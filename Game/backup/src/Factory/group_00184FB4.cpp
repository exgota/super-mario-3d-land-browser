namespace {
struct StateBlock {
    const void* resource;
    int state;
    int count;
};
}

extern "C" unsigned char dat_003CDBB8;
extern "C" unsigned char dat_003D75B8;

extern "C" void fn_00184FB4(StateBlock* self) {
    self->resource = &dat_003CDBB8;
    self->state = 0;
    self->count = 0;
}

extern "C" void fn_00242FF4(StateBlock* self) {
    self->resource = &dat_003D75B8;
    self->state = 0;
    self->count = 0;
}
