extern "C" void fn_002819A8(unsigned int* mode);
extern "C" void fn_00281978(unsigned int mode);
extern "C" int fn_00245610(unsigned int program, const char* name);
extern "C" void fn_002455B8(int location, unsigned int value);
extern "C" void fn_00245594(int location, unsigned int x, unsigned int y, unsigned int z);
extern "C" void fn_00390850(int location, unsigned int x, unsigned int y);
extern "C" void fn_002455D4(int location, float x, float y, float z, float w);
extern "C" void fn_0024CEF0(unsigned int capability, unsigned int enabled);

namespace TextureEnvironmentSetup {
struct Program { unsigned int handle; };
class ScopedMode {
    unsigned int previous;
public:
    ScopedMode() { fn_002819A8(&previous); fn_00281978(0x801); }
    ~ScopedMode() { fn_00281978(previous); }
};
}

extern "C" void fn_001D53D4(TextureEnvironmentSetup::Program* program)
{
    TextureEnvironmentSetup::ScopedMode mode;
    if (program->handle != 0x500) {
        fn_002455B8(fn_00245610(program->handle, "dmp_TexEnv[1].combineRgb"), 0x2100);
        fn_00245594(fn_00245610(program->handle, "dmp_TexEnv[1].srcRgb"), 0x6210, 0x8576, 0x8578);
        fn_00245594(fn_00245610(program->handle, "dmp_TexEnv[1].operandRgb"), 0x301, 0x300, 0x300);
        fn_00390850(fn_00245610(program->handle, "dmp_TexEnv[1].bufferInput"), 0x8578, 0x8579);
        fn_002455D4(fn_00245610(program->handle, "dmp_TexEnv[1].constRgba"), 0.08f, 0.09f, 0.1f, 1.0f);
        fn_002455B8(fn_00245610(program->handle, "dmp_TexEnv[2].combineRgb"), 0x6401);
        fn_00245594(fn_00245610(program->handle, "dmp_TexEnv[2].srcRgb"), 0x8578, 0x8579, 0x6211);
        fn_00245594(fn_00245610(program->handle, "dmp_TexEnv[2].operandRgb"), 0x301, 0x300, 0x300);
    }
    fn_0024CEF0(0x20, 1);
}
