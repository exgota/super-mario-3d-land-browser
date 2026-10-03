namespace {
struct WrapperObject {
    unsigned char pad[8];
    void* value;
};
}

extern "C" void fn_0024393C(void*, int, float);

extern "C" void fn_0016D25C(WrapperObject* self) {
    int zero = 0;
    fn_0024393C(self->value, zero, 5.0f);
}
