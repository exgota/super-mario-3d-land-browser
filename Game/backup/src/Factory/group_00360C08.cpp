namespace {
struct Object {
    unsigned char pad[0x64];
    unsigned int field;
};
extern "C" unsigned int dat_003C20D8;
extern "C" unsigned int dat_003C0684;
extern "C" int fn_0026D094(void*, unsigned int, void*);
}

extern "C" int fn_00360C08(void*, Object** p) {
    Object* object = *p;
    return fn_0026D094(object, object->field, &dat_003C20D8);
}

extern "C" int fn_00360C1C(void*, Object** p) {
    Object* object = *p;
    return fn_0026D094(object, object->field, &dat_003C20D8);
}

extern "C" int fn_0036F3E0(void*, Object** p) {
    Object* object = *p;
    return fn_0026D094(object, object->field, &dat_003C0684);
}
