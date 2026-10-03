namespace {
struct PairAt4 {
    int pad;
    int first;
    int second;
};
}

extern "C" int fn_00161464(int, int);
extern "C" int fn_001C7FC4(int, int);
extern "C" int fn_001C825C(int, int);

extern "C" int fn_0016145C(PairAt4 *p) {
    return fn_00161464(p->first, p->second);
}

extern "C" int fn_001C7FBC(PairAt4 *p) {
    return fn_001C7FC4(p->first, p->second);
}

extern "C" int fn_001C8254(PairAt4 *p) {
    return fn_001C825C(p->first, p->second);
}
