namespace al {
struct IUseNerve;
struct Nerve;
struct LiveActor;
bool isFirstStep(const IUseNerve*);
void updateNerveState(IUseNerve*);
bool isActionEnd(const LiveActor*);
void startAction(LiveActor*, const char*);
void setNerve(IUseNerve*, const Nerve*);
}

extern "C" bool fn_0032C7F8(const void*);
extern "C" void fn_00214224(void*, const char*);

extern "C" {
extern const char dat_003BEDD4[];
extern const char dat_003BEDE8[];
extern const char dat_003BF880[];
extern const char dat_003F20E0[];
extern const char dat_003BC9E4[];
extern const char dat_003F2668[];
extern const char dat_003BDB3C[];
extern const char dat_003B93C8[];
extern const char dat_003BC090[];
extern const char dat_003F236C[];
extern const char dat_003BC3B0[];
extern const char dat_003B739C[];
extern const char dat_003B74A0[];
extern const char dat_003BF664[];
extern const char dat_003F2C3C[];
extern const char dat_003F2C68[];
extern const char dat_003B6CEC[];
extern const char dat_003BEA38[];
extern const char dat_003C0EC8[];
extern const char dat_003BFC54[];
extern const char dat_003B6E24[];
extern const char dat_003B6E2C[];
extern const char dat_003B6FB4[];
extern const char dat_003BE50C[];
extern const char dat_003BE54C[];
}

#define ACTION_FN(name, pool) extern "C" void name(void*, al::IUseNerve** p) { al::IUseNerve* n = *p; if (al::isFirstStep(n)) al::startAction(reinterpret_cast<al::LiveActor*>(n), pool); }
#define NERVE_FN(name, pool) extern "C" void name(void*, al::IUseNerve** p) { al::IUseNerve* n = *p; al::updateNerveState(n); al::setNerve(n, reinterpret_cast<const al::Nerve*>(pool)); }
#define END_FN(name, pool) extern "C" void name(void*, al::IUseNerve** p) { al::IUseNerve* n = *p; if (al::isActionEnd(reinterpret_cast<al::LiveActor*>(n))) al::setNerve(n, reinterpret_cast<const al::Nerve*>(pool)); }
#define ALT_FN(name, pool) extern "C" void name(void*, al::IUseNerve** p) { al::IUseNerve* n = *p; if (al::isFirstStep(n)) fn_00214224(n, pool); }

ACTION_FN(fn_00346E8C, dat_003BEDD4)
ACTION_FN(fn_00346FBC, dat_003BEDE8)
ACTION_FN(fn_00347040, dat_003BF880)
ACTION_FN(fn_0034A180, dat_003BC9E4)
END_FN(fn_0034B00C, dat_003F2668)
ACTION_FN(fn_0034CDE8, dat_003BDB3C)
extern "C" void fn_0034EC70(void*, al::IUseNerve** p) { al::IUseNerve* n=*p; if(al::isActionEnd(reinterpret_cast<al::LiveActor*>(n))) al::startAction(reinterpret_cast<al::LiveActor*>(n),dat_003B93C8); }
ACTION_FN(fn_0034ECFC, dat_003BC090)
END_FN(fn_0034F010, dat_003F236C)
ACTION_FN(fn_0034F044, dat_003BC3B0)
ALT_FN(fn_0034F6D0, dat_003B739C)
ALT_FN(fn_0034F790, dat_003B74A0)
ACTION_FN(fn_0034F90C, dat_003BF664)
END_FN(fn_0034FC44, dat_003F2C3C)
extern "C" void fn_0034FCCC(void*, al::IUseNerve** p) { al::IUseNerve* n=*p; if(fn_0032C7F8(n)) al::setNerve(n,reinterpret_cast<const al::Nerve*>(dat_003F2C68)); }
ALT_FN(fn_00351040, dat_003B6CEC)
ACTION_FN(fn_003524F8, dat_003BEA38)
ACTION_FN(fn_00354608, dat_003C0EC8)
ACTION_FN(fn_0035466C, dat_003BFC54)
ALT_FN(fn_00356714, dat_003B6E24)
ALT_FN(fn_00356754, dat_003B6E2C)
ALT_FN(fn_0035685C, dat_003B6FB4)
ACTION_FN(fn_00356B00, dat_003BE50C)
ACTION_FN(fn_00356C1C, dat_003BE54C)
