namespace {
struct Pair32 {
    int first;
    int second;
};
}

extern "C" void fn_00132D08(void* object, int first, int second) {
    Pair32* pair = reinterpret_cast<Pair32*>(static_cast<char*>(object) + 4);
    pair->first = first;
    pair->second = second;
}

extern "C" void fn_001E22B4(void* object, int first, int second) {
    Pair32* pair = reinterpret_cast<Pair32*>(static_cast<char*>(object) + 4);
    pair->first = first;
    pair->second = second;
}
