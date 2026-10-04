namespace {
struct Object {
    unsigned char base[0x20];
    void* value;
    unsigned int state;
};
}

extern "C" void fn_0025B610(Object* object);

extern "C" void fn_00176CA0(Object* object, void* value) {
    fn_0025B610(object);
    object->value = value;
    object->state = 0;
}
