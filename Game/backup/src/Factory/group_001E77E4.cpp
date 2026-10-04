namespace {
struct Context {
    void* reserved[2];
    void* resource;
};
}

extern "C" void fn_001CC810(void*, void*, const char*, const char*);

extern "C" void fn_001E77E4(void* object, const Context* context) {
    const char* name = "ƒvƒŒƒCƒ„[‘•ü";
    fn_001CC810(object, context->resource, name, name);
}
