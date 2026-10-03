namespace {
struct Fn1928B4Object {
    unsigned char pad60[0x60];
    unsigned char subobject[1];
    unsigned char pad61[0x0F];
    float value70;
};
}

extern "C" void fn_0027434C(Fn1928B4Object *, void *, float);

extern "C" void fn_001928B4(Fn1928B4Object *self) {
    fn_0027434C(self, self->subobject, self->value70);
}
