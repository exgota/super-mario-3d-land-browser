namespace
{
struct Wrapper
{
    unsigned char unknown[0x28];
    unsigned char nerveFlag;
};
}

namespace al
{
struct IUseNerve;
struct Nerve;
void setNerve(IUseNerve *, const Nerve *);
}

extern "C" al::Nerve dat_003F1740;
extern "C" al::Nerve dat_003F1744;
extern "C" al::Nerve dat_003F1764;
extern "C" al::Nerve dat_003F174C;
extern "C" al::Nerve dat_003F1748;
extern "C" al::Nerve dat_003F1760;
extern "C" al::Nerve dat_003F1738;
extern "C" al::Nerve dat_003F1778;

#define DEFINE_WRAPPER(name, nerve) \
extern "C" void name(Wrapper *self) \
{ \
    self->nerveFlag = 1; \
    al::setNerve(reinterpret_cast<al::IUseNerve *>(self), &nerve); \
}

DEFINE_WRAPPER(fn_001B95F0, dat_003F1740)
DEFINE_WRAPPER(fn_001B9604, dat_003F1744)
DEFINE_WRAPPER(fn_001B9618, dat_003F1764)
DEFINE_WRAPPER(fn_001B9640, dat_003F174C)
DEFINE_WRAPPER(fn_001B96A0, dat_003F1748)
DEFINE_WRAPPER(fn_001B96B4, dat_003F1760)
DEFINE_WRAPPER(fn_001B96C8, dat_003F1738)
DEFINE_WRAPPER(fn_001B96F0, dat_003F1778)
