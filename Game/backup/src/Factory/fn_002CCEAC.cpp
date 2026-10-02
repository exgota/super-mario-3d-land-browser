namespace {
struct Value {
    float x, y, z, w;
};

struct Scene {
    char padding[0x52];
    bool enabled;
};

struct Interface;
struct Table {
    void (*apply)(Interface*, const void*, const Value*);
};
struct Interface {
    const Table* table;
};
struct Object {
    char padding[0x64];
    Interface interface;
};
}

namespace al {
extern Scene* getSceneObj(int);
}

extern "C" Object* fn_00227588(Scene*, const void*);
extern "C" void fn_002783BC(Value*, const Value*, float);

namespace {
void submit(const void* key, const void* arg, const Value* value) {
    Object* object = fn_00227588(al::getSceneObj(10), key);
    Interface* interface = object ? &object->interface : 0;
    interface->table->apply(interface, arg, value);
}
}

extern "C" void fn_002CCEAC(const void* key, const void* arg, const Value* value) {
    if (al::getSceneObj(10)->enabled) {
        Value copy = *value;
        fn_002783BC(&copy, &copy, -30.0f);
        submit(key, arg, &copy);
        fn_002783BC(&copy, &copy, 60.0f);
        submit(key, arg, &copy);
    } else {
        return submit(key, arg, value);
    }
}
