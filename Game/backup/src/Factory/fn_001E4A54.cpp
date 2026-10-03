namespace {
struct Owner {
    char pad[0x20];
    char* member;
};
}

extern "C" char* fn_001E4A54(Owner* self) {
    return self->member + 0x2c;
}
