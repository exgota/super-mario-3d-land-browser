namespace al {
    extern void* getSceneObj(int);
}
extern "C" void* fn_00276E50(void*);
extern "C" void* fn_00329A08(void*);

extern "C" void* fn_00276E34() {
    void* p = al::getSceneObj(0x11);
    if (p) p = static_cast<char*>(p) - 8;
    return fn_00276E50(p);
}

extern "C" void* fn_003299EC() {
    void* p = al::getSceneObj(0x11);
    if (p) p = static_cast<char*>(p) - 8;
    return fn_00329A08(p);
}
