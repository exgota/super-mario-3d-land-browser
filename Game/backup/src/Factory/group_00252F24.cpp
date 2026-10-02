namespace {
struct Result {
    unsigned char pad[0x20];
    void* value;
};
}

extern "C" Result* fn_0027768C(void*);
extern "C" void fn_001D356C(void*, void*);
extern "C" void fn_001D3774(void*, void*);
extern "C" void fn_001D3AD8(void*, void*);
extern "C" void fn_001D3764(void*, void*);
extern "C" void fn_001D34D8(void*, void*);

extern "C" void fn_00252F24(void* self) { fn_001D356C(fn_0027768C(self)->value, self); }
extern "C" void fn_002535E0(void* self) { fn_001D3774(fn_0027768C(self)->value, self); }
extern "C" void fn_002623A0(void* self) { fn_001D3AD8(fn_0027768C(self)->value, self); }
extern "C" void fn_0026CCEC(void* self) { fn_001D3764(fn_0027768C(self)->value, self); }
extern "C" void fn_0027455C(void* self) { fn_001D34D8(fn_0027768C(self)->value, self); }
