namespace {
struct PairWords {
    unsigned int first;
    unsigned int second;
};
}

extern "C" void fn_001D3F58(unsigned int* p, unsigned int value) {
    unsigned int saved = p[1];
    p[1] = value;
    p[2] = saved;
}
