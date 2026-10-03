namespace {
struct Vec3 { unsigned x, y, z; };
}
extern "C" Vec3 _ZN4sead7Vector3IfE2ezE;
extern "C" Vec3 _ZN4sead7Vector3IfE4zeroE;
extern "C" void fn_003275F8(void*, Vec3* dst) { Vec3 tmp = _ZN4sead7Vector3IfE2ezE; *dst = tmp; }
extern "C" void fn_0032760C(void*, Vec3* dst) { Vec3 tmp = _ZN4sead7Vector3IfE4zeroE; *dst = tmp; }
