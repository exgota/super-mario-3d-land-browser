namespace {
struct Owner {
    unsigned char pad[0x20];
    void* field20;
};
}
extern "C" Owner* fn_0027768C(void*);
extern "C" int fn_001D56C0(void*, void*);
extern "C" int fn_001D57D4(void*, void*);
extern "C" int fn_001D5AD8(void*, void*);
extern "C" int fn_001D5B5C(void*, void*);
extern "C" int fn_0025FB50(void*, void*);
extern "C" int fn_0025FB98(void*, void*);
extern "C" int fn_001D56A4(void* p) { return fn_001D56C0(fn_0027768C(p)->field20, p); }
extern "C" int fn_001D57B8(void* p) { return fn_001D57D4(fn_0027768C(p)->field20, p); }
extern "C" int fn_001D5ABC(void* p) { return fn_001D5AD8(fn_0027768C(p)->field20, p); }
extern "C" int fn_001D5B40(void* p) { return fn_001D5B5C(fn_0027768C(p)->field20, p); }
extern "C" int fn_0025FB34(void* p) { return fn_0025FB50(fn_0027768C(p)->field20, p); }
extern "C" int fn_0025FB7C(void* p) { return fn_0025FB98(fn_0027768C(p)->field20, p); }
