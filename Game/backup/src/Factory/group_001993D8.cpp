namespace al {
struct IUseNerve;
struct Nerve;
void setNerve(IUseNerve *, Nerve const *);
}
namespace {
struct Wrapper {
    unsigned char pad[8];
    unsigned char active;
};
}

extern "C" {
extern const unsigned char dat_003F3498[];
extern const unsigned char dat_003F3290[];
void fn_001993D8(al::IUseNerve *);
void fn_001A2288(al::IUseNerve *);
}

void fn_001993D8(al::IUseNerve *self) {
    reinterpret_cast<Wrapper *>(self)->active = 1;
    al::setNerve(self, reinterpret_cast<al::Nerve const *>(dat_003F3498));
}

void fn_001A2288(al::IUseNerve *self) {
    reinterpret_cast<Wrapper *>(self)->active = 1;
    al::setNerve(self, reinterpret_cast<al::Nerve const *>(dat_003F3290));
}
