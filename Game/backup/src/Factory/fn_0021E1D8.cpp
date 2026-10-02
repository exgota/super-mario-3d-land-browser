namespace {
struct Vector4 {
    float x, y, z, w;
};

struct SceneObject {
    char padding[0x52];
    bool enabled;
};

struct PrimaryBase {
    virtual void unused() = 0;
    char padding[0x60];
};

struct DrawingInterface {
    virtual void unused0() = 0;
    virtual void unused1() = 0;
    virtual void draw(const void*, const Vector4&) = 0;
    virtual void drawAlternate(const void*, const Vector4&) = 0;
};

struct DrawingObject : PrimaryBase, DrawingInterface {};
}

namespace al {
extern SceneObject* getSceneObj(int);
}

extern "C" DrawingObject* fn_00227588(SceneObject*, int);
extern "C" void fn_002783BC(Vector4*, const Vector4*, float);

namespace {
inline DrawingInterface* getDrawing(int index)
{
    return fn_00227588(al::getSceneObj(10), index);
}

void draw(int index, const void* context, const Vector4& vector)
{
    getDrawing(index)->draw(context, vector);
}

extern "C" inline void fn_0021E2D0(int index, const void* context, const Vector4& vector)
{
    getDrawing(index)->drawAlternate(context, vector);
}
}

extern "C" void fn_0021E1D8(int index, const void* context, const Vector4& vector)
{
    if (al::getSceneObj(10)->enabled) {
        Vector4 copy = vector;
        fn_002783BC(&copy, &copy, -30.0f);
        draw(index, context, copy);
        fn_002783BC(&copy, &copy, 60.0f);
        draw(index, context, copy);
    } else {
        return fn_0021E2D0(index, context, vector);
    }
}
