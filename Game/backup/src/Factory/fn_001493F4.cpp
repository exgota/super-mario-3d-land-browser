namespace {
typedef unsigned int u32;
struct FourWords { u32 a, b, c, d; };
}

extern "C" void fn_001493F4(u32* destination, u32 a, u32 b, u32 c, u32 d) {
    FourWords value;
    value.a = a;
    value.b = b;
    value.c = c;
    value.d = d;
    *reinterpret_cast<FourWords*>(destination) = value;
}
