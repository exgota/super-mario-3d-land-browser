namespace {
struct ResourceDeviceState {
    unsigned int unknown_00;
    unsigned int unknown_04;
    void* device;
};
}

extern "C" {
extern ResourceDeviceState dat_003EF15C;
void CFLi_FinalizeDevice(void* device);

void CFLi_FinalizeResourceDevice() {
    CFLi_FinalizeDevice(dat_003EF15C.device);
    dat_003EF15C.device = 0;
}
}
