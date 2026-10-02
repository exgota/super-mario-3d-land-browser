namespace al { struct IUseNerve; struct Nerve; struct LiveActor; }
namespace {
using al::IUseNerve; using al::Nerve; using al::LiveActor;
extern "C" unsigned char dat_003F334C;
extern "C" unsigned char dat_003F3358;
extern "C" unsigned char dat_003F16EC;
extern "C" unsigned char dat_003F1700;
extern "C" unsigned char dat_003F18B4;
extern "C" unsigned char dat_003F32B0;
extern "C" unsigned char dat_003F32EC;
extern "C" unsigned char dat_003F32E0;
extern "C" unsigned char dat_003F35C4;
extern "C" unsigned char dat_003BDF20;
}
namespace al {
void updateNerveStateAndNextNerve(IUseNerve*, const Nerve*);
void setNerve(IUseNerve*, const Nerve*);
void startAction(LiveActor*, const char*);
}
#define WRAP_UPDATE(name, data) extern "C" void name(void*, void** p) { al::updateNerveStateAndNextNerve(reinterpret_cast<IUseNerve*>(*p), reinterpret_cast<const Nerve*>(&data)); }
#define WRAP_SET(name, data) extern "C" void name(void*, void** p) { al::setNerve(reinterpret_cast<IUseNerve*>(*p), reinterpret_cast<const Nerve*>(&data)); }
WRAP_UPDATE(fn_00363114, dat_003F334C)
WRAP_UPDATE(fn_00363178, dat_003F3358)
WRAP_UPDATE(fn_0036482C, dat_003F16EC)
WRAP_UPDATE(fn_0036483C, dat_003F1700)
WRAP_UPDATE(fn_0036484C, dat_003F1700)
WRAP_UPDATE(fn_0036485C, dat_003F1700)
WRAP_SET(fn_003654AC, dat_003F18B4)
WRAP_UPDATE(fn_003663CC, dat_003F32B0)
WRAP_UPDATE(fn_003663DC, dat_003F32B0)
WRAP_UPDATE(fn_003663EC, dat_003F32B0)
WRAP_UPDATE(fn_003663FC, dat_003F32B0)
WRAP_UPDATE(fn_0036640C, dat_003F32B0)
WRAP_UPDATE(fn_0036641C, dat_003F32B0)
WRAP_UPDATE(fn_0036642C, dat_003F32EC)
WRAP_UPDATE(fn_00366484, dat_003F32B0)
WRAP_UPDATE(fn_00366494, dat_003F32B0)
WRAP_UPDATE(fn_003664A4, dat_003F32B0)
WRAP_UPDATE(fn_003664B4, dat_003F32B0)
WRAP_UPDATE(fn_003664C4, dat_003F32E0)
WRAP_UPDATE(fn_003664D4, dat_003F32B0)
WRAP_SET(fn_0036759C, dat_003F35C4)
extern "C" void fn_0036CCE8(void*, void** p) { al::startAction(reinterpret_cast<LiveActor*>(*p), reinterpret_cast<const char*>(&dat_003BDF20)); }
