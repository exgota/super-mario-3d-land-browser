namespace {
extern "C" void fn_002850DC(void*, void*);
extern "C" void fn_001F5A54(void*, void*);
extern "C" unsigned char dat_003A7A98[];
extern "C" unsigned char dat_003EF07C[];
extern "C" unsigned char dat_003A98EC[];
extern "C" unsigned char dat_003F0378[];
}

extern "C" void CFLi_InitializeRenderSettings() {
    fn_002850DC(dat_003EF07C, dat_003A7A98);
}

extern "C" void fn_001F57B0() {
    fn_001F5A54(dat_003F0378, dat_003A98EC);
}
