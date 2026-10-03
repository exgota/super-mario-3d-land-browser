namespace {
struct Holder {
    void *pad[3];
    void *value;
};
}

extern "C" void fn_002589F8(void *);
extern "C" void fn_0025015C(void *);
extern "C" void fn_0024C8C0(void *);

extern "C" void fn_0018A378(Holder *p) { fn_002589F8(p->value); }
extern "C" void fn_00242EF4(Holder *p) { fn_0025015C(p->value); }
extern "C" void fn_002BD920(Holder *p) { fn_0024C8C0(p->value); }
