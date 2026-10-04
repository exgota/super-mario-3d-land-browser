namespace {
struct Fn001A9554Nested {
    char padding[8];
    signed char value;
};
struct Fn001A9554Object {
    char padding[4];
    Fn001A9554Nested *nested;
};
}

extern "C" signed char fn_001A9554(Fn001A9554Object *self) {
    return self->nested->value;
}
