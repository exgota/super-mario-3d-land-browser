namespace {
struct Vector3 {
    float x;
    float y;
    float z;
};

struct VectorHolder {
    void* vtable;
    Vector3* value;
};
}

extern "C" void fn_0013E050(VectorHolder* self, const Vector3* source) {
    Vector3* destination = self->value;
    destination->x = source->x;
    destination->y = source->y;
    destination->z = source->z;
}
