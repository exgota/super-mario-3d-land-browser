namespace {
struct Object {
    unsigned char pad[0x68];
    unsigned int value;
};
}

extern "C" unsigned int fn_0027FF40(Object *, unsigned int);

#define WRAPPER(name) extern "C" unsigned int name(Object *object) { return fn_0027FF40(object, object->value); }
WRAPPER(fn_0011A384)
WRAPPER(fn_0012DA1C)
WRAPPER(fn_0013DBF0)
WRAPPER(fn_00157C04)
WRAPPER(fn_00177238)
WRAPPER(fn_003237A0)
WRAPPER(fn_00323A4C)
