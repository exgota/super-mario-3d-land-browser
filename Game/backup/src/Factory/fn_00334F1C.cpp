namespace {
struct Vec3 {
    float x, y, z;
    Vec3(float b, float c, float a) : x(a), y(b), z(c) {}
};

struct Object {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void slot3() = 0;
    virtual void slot4() = 0;
    virtual const Vec3& getVector() = 0;

    Vec3 first;
    Vec3 second;
};
}

extern "C" void fn_00277884(void*, const Vec3*, const Vec3*, const Vec3*);

extern "C" void fn_00334F1C(Object* object, void* output) {
    const Vec3& vector = object->getVector();
    Vec3 negative(-vector.y, -vector.z, -vector.x);
    fn_00277884(output, &object->second, &negative, &object->first);
}
