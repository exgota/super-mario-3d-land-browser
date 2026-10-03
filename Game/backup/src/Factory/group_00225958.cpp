namespace al {
int isPadTrigger(int, int);
}

extern "C" int fn_00250EE0(int, int);

extern "C" int fn_00225958(int pad) {
    return al::isPadTrigger(pad, 0x4000);
}

extern "C" int fn_002734A0(int pad) {
    return fn_00250EE0(pad, 0x4000);
}
