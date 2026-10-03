namespace {
struct SetterTarget {
    unsigned char padding[0x84];
    unsigned value;
};
}

extern "C" void fn_0011B528(SetterTarget* self, unsigned value) {
    self->value = value;
}

extern "C" void fn_0011F1BC(SetterTarget* self, unsigned value) {
    self->value = value;
}

extern "C" void fn_0013FCD4(SetterTarget* self, unsigned value) {
    self->value = value;
}
