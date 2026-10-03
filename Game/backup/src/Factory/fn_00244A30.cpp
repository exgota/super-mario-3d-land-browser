namespace {
struct Source {
    unsigned int unused;
    void *value;
};
}

extern "C" void fn_00244A30(void *destination, const Source *source) {
    *static_cast<void **>(destination) = source->value;
}
