namespace {
typedef void* Ptr;
typedef void** Arg;
}
extern "C" Ptr fn_001665B8(Ptr);
extern "C" Ptr fn_002D7410(Ptr);
extern "C" Ptr fn_00214408(Ptr);
extern "C" Ptr fn_00303C98(Ptr);
extern "C" Ptr fn_00316230(Ptr);
extern "C" Ptr fn_001187F4(Ptr);
extern "C" Ptr fn_00125778(Ptr);
extern "C" Ptr fn_00125AD4(Ptr);
extern "C" Ptr fn_0012876C(Ptr);
extern "C" Ptr fn_00213958(Ptr);
extern "C" Ptr fn_0013A804(Ptr);
extern "C" Ptr fn_00146428(Ptr);
extern "C" Ptr fn_0014A03C(Ptr);
extern "C" Ptr fn_0014B6A8(Ptr);
extern "C" Ptr fn_0014ACFC(Ptr);
extern "C" Ptr fn_00213544(Ptr);
namespace al { struct IUseNerve; Ptr updateNerveState(IUseNerve*); }
#define WRAP(name, target) extern "C" Ptr name(Ptr, Arg p) { return target(*p); }
WRAP(fn_0032C354, fn_001665B8)
WRAP(fn_00345300, fn_002D7410)
WRAP(fn_00347070, fn_00214408)
WRAP(fn_00347078, fn_00214408)
WRAP(fn_003473F8, fn_00303C98)
WRAP(fn_0034D420, fn_00316230)
WRAP(fn_00350CB0, fn_001187F4)
WRAP(fn_0035419C, fn_00125778)
WRAP(fn_00354298, fn_00125AD4)
WRAP(fn_003555DC, fn_0012876C)
WRAP(fn_00357674, fn_00213958)
WRAP(fn_0035767C, fn_00213958)
WRAP(fn_003593B4, fn_0013A804)
WRAP(fn_0035BF54, fn_00146428)
WRAP(fn_0035C734, fn_0014A03C)
WRAP(fn_0035C960, fn_0014B6A8)
WRAP(fn_0035CF40, fn_0014ACFC)
WRAP(fn_0035E61C, fn_00213544)
WRAP(fn_0035E624, fn_00213544)
#undef WRAP
#define AL_WRAP(name) extern "C" Ptr name(Ptr, Arg p) { return al::updateNerveState(reinterpret_cast<al::IUseNerve*>(*p)); }
AL_WRAP(fn_0034E278)
AL_WRAP(fn_0034F6B8)
AL_WRAP(fn_00350928)
AL_WRAP(fn_00351038)
AL_WRAP(fn_00359D08)
AL_WRAP(fn_0035E168)
