namespace {
struct Nerve {};
struct IUseNerve {};
}
namespace al {
struct Nerve {};
struct IUseNerve {};
void updateNerveStateAndNextNerve(IUseNerve *, const Nerve *);
}
extern "C" {
extern const al::Nerve dat_003F2160;
extern const al::Nerve dat_003F1D58;
extern const al::Nerve dat_003F21D0;
extern const al::Nerve dat_003F1DBC;
extern const al::Nerve dat_003F16E8;
extern const al::Nerve dat_003F16D0;
extern const al::Nerve dat_003F1714;
extern const al::Nerve dat_003F15A4;
extern const al::Nerve dat_003F15D0;
extern const al::Nerve dat_003F15E4;
extern const al::Nerve dat_003F2094;
extern const al::Nerve dat_003F2084;
extern const al::Nerve dat_003F1624;
extern const al::Nerve dat_003F1610;
extern const al::Nerve dat_003F1658;
extern const al::Nerve dat_003F1640;
#define THUNK(name, data) void name(void *, const al::Nerve **p) { al::updateNerveStateAndNextNerve((al::IUseNerve *)*p, &data); }
THUNK(fn_003460E0, dat_003F2160)
THUNK(fn_0034725C, dat_003F1D58)
THUNK(fn_003487E8, dat_003F21D0)
THUNK(fn_0034897C, dat_003F21D0)
THUNK(fn_00348FD4, dat_003F1DBC)
THUNK(fn_0034F6C0, dat_003F16E8)
THUNK(fn_0034F700, dat_003F16D0)
THUNK(fn_0034F710, dat_003F16E8)
THUNK(fn_0034F8B8, dat_003F1714)
THUNK(fn_00350918, dat_003F15A4)
THUNK(fn_00350930, dat_003F15A4)
THUNK(fn_00351070, dat_003F15D0)
THUNK(fn_00351080, dat_003F15E4)
THUNK(fn_00351090, dat_003F15E4)
THUNK(fn_003510A0, dat_003F15E4)
THUNK(fn_0035129C, dat_003F2094)
THUNK(fn_00351694, dat_003F2084)
THUNK(fn_00356744, dat_003F1624)
THUNK(fn_003567D8, dat_003F1610)
THUNK(fn_003567E8, dat_003F1624)
THUNK(fn_003567F8, dat_003F1624)
THUNK(fn_0035688C, dat_003F1658)
THUNK(fn_003568F0, dat_003F1658)
THUNK(fn_00356900, dat_003F1640)
THUNK(fn_00356910, dat_003F1658)
#undef THUNK
}
