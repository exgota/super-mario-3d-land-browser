namespace {
struct Object {
    unsigned char pad[12];
    unsigned int value;
};
}

extern "C" void fn_002ABCEC(Object* object) {
    object->value = 0;
}
