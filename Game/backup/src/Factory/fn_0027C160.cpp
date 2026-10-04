namespace {
struct Fn0027C160Target {
    unsigned char pad[0x10];
    unsigned int value;
};
struct Fn0027C160Owner {
    unsigned char pad[0x24];
    Fn0027C160Target* target;
};
}

extern "C" void fn_0027C160(Fn0027C160Owner* self, unsigned int value) {
    self->target->value = value;
}
