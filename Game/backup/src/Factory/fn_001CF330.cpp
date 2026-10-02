namespace {
extern "C" void* fn_0024A89C(int);
}

extern "C" void fn_001CF330(void* self) {
    *reinterpret_cast<void**>(static_cast<char*>(self) + 0x8c) = fn_0024A89C(0x402);
    for (;;) {
        void** vtable = *reinterpret_cast<void***>(self);
        reinterpret_cast<void (*)(void*)>(vtable[0x5c / 4])(self);
    }
}
