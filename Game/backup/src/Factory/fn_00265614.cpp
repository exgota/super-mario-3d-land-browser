namespace {
struct Args {
    int a;
    int b;
    int c;
};
}

extern "C" int fn_0026561C(int, int, int);

extern "C" int fn_00265614(const Args *args) {
    return fn_0026561C(args->a, args->b, args->c);
}
