namespace al {
bool isPadTrigger(int, int);
}

extern "C" bool fn_0024CA44(int pad) {
    return al::isPadTrigger(pad, 0x800);
}
