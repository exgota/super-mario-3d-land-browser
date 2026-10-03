namespace {
extern "C" unsigned char dat_003EF054[];
extern "C" unsigned char dat_003E23F0[];
}

extern "C" unsigned char CFLi_IsNandSharedExtMounted() {
    return dat_003EF054[1];
}

extern "C" unsigned char fn_0024AC5C() {
    return dat_003E23F0[1];
}
