namespace {
struct Inner {
    unsigned int value;
};

struct Outer {
    unsigned int unused;
    Inner* inner;
};
}

extern "C" unsigned int fn_002589F8(unsigned int);

extern "C" unsigned int fn_002589EC(Outer* object) {
    return fn_002589F8(object->inner->value);
}
