namespace {
struct Receiver {
    unsigned char pad[0x8c];
    int first;
    int second;
    int third;
};
}

extern "C" void fn_00288D34(Receiver *, int, int, int);

extern "C" void fn_001CB65C(Receiver *self, int a, int b, int c) {
    self->first = a;
    self->second = b;
    self->third = c;
    fn_00288D34(self, a, b, c);
}
