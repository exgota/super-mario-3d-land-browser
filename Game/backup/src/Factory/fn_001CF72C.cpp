namespace {
struct ThreeWords {
    unsigned int first;
    unsigned int second;
    unsigned int third;
};
}

extern "C" void fn_001CF72C(ThreeWords* out, unsigned int first, unsigned int second) {
    ThreeWords value = {first, second, 0};
    *out = value;
}
