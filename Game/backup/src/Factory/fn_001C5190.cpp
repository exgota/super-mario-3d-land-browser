namespace {
struct Inner {
    unsigned value;
};

struct Object {
    unsigned padding[2];
    Inner* inner;
};
}

extern "C" unsigned fn_001C519C(unsigned);

extern "C" unsigned fn_001C5190(Object* object) {
    return fn_001C519C(object->inner->value);
}
