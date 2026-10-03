namespace al {
struct IUseNerve {};
struct Nerve {};
void setNerve(IUseNerve *, const Nerve *);
}

extern "C" al::Nerve dat_003F173C;
extern "C" al::Nerve dat_003F175C;

namespace {
struct StateOwner {
    unsigned char padding[0x28];
    unsigned char state;
};
}

extern "C" void fn_001B962C(StateOwner *self) {
    self->state = 0;
    al::setNerve(reinterpret_cast<al::IUseNerve *>(self), &dat_003F173C);
}

extern "C" void fn_001B96DC(StateOwner *self) {
    self->state = 0;
    al::setNerve(reinterpret_cast<al::IUseNerve *>(self), &dat_003F175C);
}
