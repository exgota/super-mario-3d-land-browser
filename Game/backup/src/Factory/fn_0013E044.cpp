namespace {
struct Pair {
    unsigned first;
    unsigned second;
};
}

extern "C" void fn_0013E044(void* object, unsigned first, unsigned second) {
    Pair value = { first, second };
    *reinterpret_cast<Pair*>(static_cast<char*>(object) + 0x10) = value;
}
