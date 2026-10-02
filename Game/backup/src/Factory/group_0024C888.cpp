namespace {
struct PointerHolder {
    void *value;
};
}

extern "C" void fn_002BDD64(void *);
extern "C" void fn_0022B5E4(void *);
extern "C" void fn_002BDD5C(void *);
extern "C" void fn_00338680(void *);

extern "C" void fn_0024C888(PointerHolder *p) {
    if (p->value)
        fn_002BDD64(p->value);
}

extern "C" void fn_0024C8A4(PointerHolder *p) {
    if (p->value)
        fn_0022B5E4(p->value);
}

extern "C" void fn_002DA370(PointerHolder *p) {
    if (p->value)
        fn_002BDD5C(p->value);
}

extern "C" void fn_003409AC(PointerHolder *p) {
    if (p->value)
        fn_00338680(p->value);
}
