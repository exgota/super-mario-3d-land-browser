namespace {
struct Entry { int words[3]; };
struct Inner { unsigned char pad[8]; Entry* entries; };
struct Outer { unsigned char pad[4]; Inner* inner; };
}

extern "C" Entry* fn_0032EAC8(Outer* self, int index) {
    return self->inner->entries + index;
}

extern "C" Entry* fn_0032ED2C(Outer* self, int index) {
    return self->inner->entries + index;
}
