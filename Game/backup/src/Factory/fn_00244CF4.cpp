namespace {
struct Result {
    unsigned int padding[7];
    unsigned int value;
};
}

extern "C" Result* fn_0027AD14();

extern "C" unsigned int fn_00244CF4() {
    return fn_0027AD14()->value;
}
