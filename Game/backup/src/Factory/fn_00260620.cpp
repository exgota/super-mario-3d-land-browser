namespace {
extern "C" int fn_00277CEC(void*);
extern "C" char dat_003F1C80[];
}

extern "C" const char* fn_00260620(void* object) {
    return fn_00277CEC(object)
        ? reinterpret_cast<const char*>(0x0042F2C0)
        : dat_003F1C80;
}
