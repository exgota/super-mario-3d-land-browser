namespace al { struct Nerve {}; }
namespace {
struct Host {
    unsigned char pad[8];
    unsigned char active;
};
extern "C" al::Nerve dat_003F3510;
extern "C" al::Nerve dat_003F33AC;
extern "C" al::Nerve dat_003F33B8;
extern "C" al::Nerve dat_003F359C;
extern "C" al::Nerve dat_003F30A8;
extern "C" al::Nerve dat_003F30E4;
extern "C" al::Nerve dat_003F00E8;
extern "C" al::Nerve dat_003F2C64;
extern "C" al::Nerve dat_003F3368;
extern "C" al::Nerve dat_003F30C8;
extern "C" al::Nerve dat_003F3578;
extern "C" al::Nerve dat_003F3590;
extern "C" al::Nerve dat_003F00A4;
extern "C" al::Nerve dat_003F19F0;
extern "C" al::Nerve dat_003F2F9C;
extern "C" al::Nerve dat_003F003C;
}
namespace al {
struct IUseNerve;
void setNerve(IUseNerve *, const Nerve *);
}
#define WRAP(name, dat) extern "C" void name(Host *h) { h->active = 0; al::setNerve(reinterpret_cast<al::IUseNerve *>(h), &dat); }
WRAP(fn_0014CE7C, dat_003F3510)
WRAP(fn_0014D668, dat_003F33AC)
WRAP(fn_0014DA3C, dat_003F33B8)
WRAP(fn_0015EF90, dat_003F359C)
WRAP(fn_001861CC, dat_003F30A8)
WRAP(fn_00186260, dat_003F30E4)
WRAP(fn_00190474, dat_003F00E8)
WRAP(fn_00190A7C, dat_003F2C64)
WRAP(fn_00195CB8, dat_003F3368)
WRAP(fn_001A15FC, dat_003F30C8)
WRAP(fn_001A349C, dat_003F3578)
WRAP(fn_001A3534, dat_003F3590)
WRAP(fn_001A52C0, dat_003F00A4)
WRAP(fn_001A5F78, dat_003F19F0)
WRAP(fn_001A8FAC, dat_003F2F9C)
WRAP(fn_001B5FA4, dat_003F003C)
