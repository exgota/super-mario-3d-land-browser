namespace al {
extern int isPadTrigger(int, int);
}
extern "C" int fn_00250EE0(int, int);

extern "C" int fn_00225960(int value) {
    return al::isPadTrigger(value, 0x2000);
}

extern "C" int fn_002734A8(int value) {
    return fn_00250EE0(value, 0x2000);
}
