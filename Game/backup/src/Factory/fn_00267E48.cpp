namespace {
struct FourFloats { float a, b, c, d; };
}

extern "C" void fn_00267E50(int, float, float, float, float);

extern "C" void fn_00267E48(int value, FourFloats* floats) {
    fn_00267E50(value, floats->a, floats->b, floats->c, floats->d);
}
