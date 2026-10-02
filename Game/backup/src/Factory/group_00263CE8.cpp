namespace {
struct Triple { volatile unsigned a, b, c; };
}

extern "C" void fn_0027D5C4(Triple*);

extern "C" void fn_00263CE8(Triple* dst, const Triple* src) {
    dst->a = src->a;
    dst->b = src->b;
    dst->c = src->c;
    fn_0027D5C4(dst);
}
