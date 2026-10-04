namespace {
struct Vector3 {
    float x;
    float y;
    float z;
};

struct Object {
    unsigned char reserved[0x14];
    Vector3 vector;
};
}

extern "C" const Vector3* fn_0026CCD0(Object*);

extern "C" void fn_00179EDC(Object* self) {
    Vector3* destination = &self->vector;
    const Vector3* source = fn_0026CCD0(self);
    *destination = *source;
}
