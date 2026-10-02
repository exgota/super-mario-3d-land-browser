namespace {
typedef void* OpaqueObject;
}

extern "C" int fn_00267C1C(OpaqueObject);
extern "C" int fn_00253420(OpaqueObject);

extern "C" int fn_001C9520(OpaqueObject* object) {
    OpaqueObject value = *object;
    if (value == 0)
        return -1;
    return fn_00267C1C(value);
}

extern "C" int fn_001C9538(OpaqueObject* object) {
    OpaqueObject value = *object;
    if (value == 0)
        return -1;
    return fn_00253420(value);
}

extern "C" int fn_001E1DE0(OpaqueObject* object) {
    OpaqueObject value = *object;
    if (value == 0)
        return -1;
    return fn_00253420(value);
}

extern "C" int fn_001E1DF8(OpaqueObject* object) {
    OpaqueObject value = *object;
    if (value == 0)
        return -1;
    return fn_00267C1C(value);
}
