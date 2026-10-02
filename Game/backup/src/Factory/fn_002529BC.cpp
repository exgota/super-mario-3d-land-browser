namespace {
struct Vec2 {
    float x;
    float y;
};

struct Vec3 {
    float x;
    float y;
    float z;
};

struct Object {
    char padding0[0x14];
    float height;
    char padding1[0x0c];
    int counter;
};
}

namespace rp {
extern const Vec3& getPlayerPos();
}

extern "C" void fn_002711A8(Vec2&, const Vec3&);
extern "C" bool fn_002786F4();
extern "C" bool fn_002D19B8();
extern "C" bool fn_002D1984();
extern "C" const Vec3& fn_002D1950();
extern "C" bool fn_002660DC();
extern "C" int fn_0025847C();

extern "C" bool fn_002529BC(Object* self) {
    Vec2 position;
    position.x = 0.0f;
    position.y = 0.0f;
    fn_002711A8(position, rp::getPlayerPos());
    if (position.y > 150.0f || position.y < -110.0f)
        return true;
    if (fn_002786F4())
        return true;
    if (fn_002D19B8())
        return true;
    if (fn_002D1984() && self->height > rp::getPlayerPos().y)
        return true;
    float speed = fn_002D1950().y;
    if ((speed > 0.0f ? speed : -speed) > 44.0f)
        return true;
    if (fn_002660DC())
        return true;
    if (self->counter > 25)
        return true;
    return fn_0025847C() ? true : false;
}
