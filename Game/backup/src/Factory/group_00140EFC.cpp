namespace {
struct AccessorObject {
    unsigned char padding[0x2c];
    void *value;
};
}

extern "C" AccessorObject sPhotoScenarioNames;
extern "C" AccessorObject dat_003EFEE4;

extern "C" void *fn_00140EFC() {
    return sPhotoScenarioNames.value;
}

extern "C" void *fn_0032A6BC() {
    return dat_003EFEE4.value;
}
