namespace {
struct ObjectView {
    char padding[0x3d];
    signed char value;
};
}

extern "C" signed char fn_00326C88(const ObjectView* object) {
    return object->value;
}
