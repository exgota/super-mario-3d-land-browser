namespace {
struct Fn002BDD5CObject {
    unsigned char unknown_0000[0xB8];
    float value;
};
}

extern "C" void fn_002BDD5C(Fn002BDD5CObject* object, float value) {
    object->value = value;
}
