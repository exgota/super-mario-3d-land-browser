namespace {
struct Adjusted { unsigned char pad[4]; };
}

extern "C" void fn_0024C888(void*);
extern "C" void fn_0024C8A4(void*);
extern "C" void fn_0024C8C0(void*);
extern "C" void fn_00283D78(void*, void*);
extern "C" void fn_002B9C00(void*);
extern "C" void fn_002BA98C(void*);
extern "C" void fn_002DCB58(void*);

namespace nn { namespace os {
class LightEvent { public: void Signal(); };
}}

extern "C" void fn_0024C880(void* p) { fn_0024C888((char*)p + 4); }
extern "C" void fn_0024C89C(void* p) { fn_0024C8A4((char*)p + 4); }
extern "C" void fn_0024C8B8(void* p) { fn_0024C8C0((char*)p + 4); }
extern "C" void fn_00283D70(void* p, void* allocation) { fn_00283D78((char*)p + 4, allocation); }
extern "C" void fn_00291538(void* p) { ((nn::os::LightEvent*)((char*)p + 4))->Signal(); }
extern "C" void fn_002B9BF8(void* p) { fn_002B9C00((char*)p + 4); }
extern "C" void fn_002BA984(void* p) { fn_002BA98C((char*)p + 4); }
extern "C" void fn_002DCB50(void* p) { fn_002DCB58((char*)p + 4); }
