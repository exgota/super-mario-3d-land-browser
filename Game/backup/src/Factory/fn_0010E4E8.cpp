namespace {
struct Device;
struct DeviceVTable {
    void* unused;
    void (*finalize)(Device*);
};
struct Device {
    DeviceVTable* vtable;
};
}

extern "C" void CFLi_FinalizeDevice(Device* device)
{
    device->vtable->finalize(device);
}
