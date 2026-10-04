extern "C" void* fn_0026B4CC(void*, void*, unsigned int);

extern "C" void* fn_0026B4C0(void* self, void* arg1, unsigned int) {
    void* resource = *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(self) + 0x20);
    return fn_0026B4CC(resource, arg1, 0x60);
}
