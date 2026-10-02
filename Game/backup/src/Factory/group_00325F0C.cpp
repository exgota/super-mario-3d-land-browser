namespace {
typedef int Result;
}

extern "C" Result fn_00286770();
extern "C" Result fn_00285D1C();

extern "C" int fn_00325F0C() {
    return fn_00286770() != 0;
}

extern "C" int fn_00326000() {
    return fn_00285D1C() != 0;
}
