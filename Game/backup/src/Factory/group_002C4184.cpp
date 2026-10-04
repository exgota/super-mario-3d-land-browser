namespace {
struct Vector3 {
    float x;
    float y;
    float z;
};

struct Object {
    unsigned char padding0[9];
    bool active;
    unsigned char padding1[0x42];
    Vector3 position;
};

struct Receiver;
}

extern "C" Receiver* fn_0024B56C(Object*);
extern "C" void fn_00228FE8(Receiver*, const Vector3*);

extern "C" void fn_002C4184(Object* object) {
    Receiver* receiver = fn_0024B56C(object);
    fn_00228FE8(receiver, &object->position);
    object->active = true;
}

extern "C" void fn_002C6864(Object* object) {
    Receiver* receiver = fn_0024B56C(object);
    fn_00228FE8(receiver, &object->position);
    object->active = true;
}
