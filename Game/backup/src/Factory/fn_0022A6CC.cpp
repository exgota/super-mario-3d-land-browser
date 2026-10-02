namespace {
extern "C" int fn_00232390(int, int*);
}

extern "C" int fn_0022A6CC(int a, int* b) {
    return fn_00232390(a, b + 1);
}
