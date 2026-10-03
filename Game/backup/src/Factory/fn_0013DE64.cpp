namespace {

struct Vector3 {
    float x;
    float y;
    float z;
};

struct VectorTarget {
    unsigned char unknown[0x24];
    Vector3 value;
};

struct VectorProxy {
    unsigned int unknown;
    VectorTarget* target;
};

}

extern "C" void fn_0013DE64(VectorProxy* self, const Vector3* value) {
    VectorTarget* target = self->target;
    target->value.x = value->x;
    target->value.y = value->y;
    target->value.z = value->z;
}
