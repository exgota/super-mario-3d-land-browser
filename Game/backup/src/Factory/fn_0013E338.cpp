namespace {
struct Vector3 {
    float x, y, z;
};

struct State {
    unsigned char reserved0[0xd4];
    Vector3 first;
    unsigned char reserved1[0x20c - 0xd4 - sizeof(Vector3)];
    Vector3 second;
};

struct Link {
    unsigned char reserved[0x10];
    State* state;
};

struct Object;
struct VTable {
    void (*reserved[16])();
    void (*fn_0013E358)(Object*, Vector3*, Vector3*, int);
};

struct Object {
    VTable* vtable;
    unsigned char reserved[0x1c];
    Link* link;
};
}

extern "C" void fn_0013E338(Object* self) {
    State* state = self->link->state;
    return self->vtable->fn_0013E358(self, &state->second, &state->first, 0);
}
