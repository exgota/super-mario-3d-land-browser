namespace {
struct Context {
    void* reserved[2];
    void* resource;
};
}

extern "C" void fn_001CC810(void*, void*, const char*, const char*);

extern "C" void fn_0026F5CC(void* object, const Context* context) {
    const char* name = "\x93G[Movement]";
    fn_001CC810(object, context->resource, name, name);
}
