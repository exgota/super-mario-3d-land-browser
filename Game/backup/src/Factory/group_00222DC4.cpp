namespace sead {
struct IDisposer { ~IDisposer(); };
struct UlcdTask { ~UlcdTask(); };
}

extern "C" char dat_003D9C74;
extern "C" char dat_003D8CC8;
extern "C" char dat_003D4330;
extern "C" char dat_003DA394;
extern "C" char dat_003DA3A4;
extern "C" void fn_002BAD3C(void*);
extern "C" void fn_00205444(void*);

extern "C" void fn_00222DC4(void* self) {
    *reinterpret_cast<void**>(self) = &dat_003D9C74;
    static_cast<sead::IDisposer*>(self)->~IDisposer();
}

extern "C" void fn_002BC498(void* self) {
    *reinterpret_cast<void**>(self) = &dat_003D8CC8;
    fn_002BAD3C(self);
}

extern "C" void fn_00318A1C(void* self) {
    *reinterpret_cast<void**>(self) = &dat_003D4330;
    static_cast<sead::UlcdTask*>(self)->~UlcdTask();
}

extern "C" void fn_0039A920(void* self) {
    *reinterpret_cast<void**>(self) = &dat_003DA394;
    fn_00205444(self);
}

extern "C" void fn_0039A930(void* self) {
    *reinterpret_cast<void**>(self) = &dat_003DA3A4;
    fn_00205444(self);
}
