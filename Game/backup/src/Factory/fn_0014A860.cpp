namespace {
struct Object {
    int value;
    int padding;
    int target;
};
}

extern "C" int fn_002CFCD8(int);

extern "C" int fn_0014A860(Object* object) {
    int value = object->target;
    if (value == 0)
        return 1;
    return fn_002CFCD8(value);
}
