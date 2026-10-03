namespace {
struct Result {
    unsigned int reserved[8];
    void* value;
};
}

extern "C" Result* fn_0027768C(void*);
extern "C" void* fn_001D3890(void*);
extern "C" void* fn_001D3AA8(void*);

extern "C" void* fn_00262328(void* self) {
    return fn_001D3890(fn_0027768C(self)->value);
}

extern "C" void* fn_00265F04(void* self) {
    return fn_001D3AA8(fn_0027768C(self)->value);
}
