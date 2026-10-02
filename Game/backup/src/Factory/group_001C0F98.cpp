namespace {
struct ObjectLayout {
    int padding[3];
    int value;
};
}

extern "C" int fn_001C0F98(const void* object) {
    return static_cast<const ObjectLayout*>(object)->value > 0;
}

extern "C" int fn_003361FC(const void* object) {
    return static_cast<const ObjectLayout*>(object)->value > 0;
}

extern "C" int fn_00336678(const void* object) {
    return static_cast<const ObjectLayout*>(object)->value > 0;
}

extern "C" int fn_003366CC(const void* object) {
    return static_cast<const ObjectLayout*>(object)->value > 0;
}

extern "C" int fn_003369DC(const void* object) {
    return static_cast<const ObjectLayout*>(object)->value > 0;
}
