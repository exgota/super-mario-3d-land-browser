namespace {
struct Object_00377CE0 {
    char padding[0x5A];
    signed char value;
};
}

extern "C" int fn_00377CE0(const Object_00377CE0* object)
{
    return object->value;
}
