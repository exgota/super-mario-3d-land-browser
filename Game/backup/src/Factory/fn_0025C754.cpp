namespace {
struct Object_0025C754 {
    unsigned char pad[8];
    char* data;
};
}

extern "C" char* fn_0025C754(Object_0025C754* self) {
    return self->data + 0x4c;
}
