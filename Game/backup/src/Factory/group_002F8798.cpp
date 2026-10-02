namespace {
struct ActorLike {
    unsigned char pad[0x60];
    unsigned char quat[0x10];
};
}

namespace al {
class LiveActor;
}
namespace sead {
template <typename T> struct Quat;
}
namespace al {
void setQuat(LiveActor *, const sead::Quat<float> &);
class LiveActor { public: void startClipped(); };
}

extern "C" void fn_002F8798(al::LiveActor *self) {
    al::setQuat(self, *reinterpret_cast<const sead::Quat<float> *>(reinterpret_cast<unsigned char *>(self) + 0x60));
    self->startClipped();
}
extern "C" void fn_003115B4(al::LiveActor *self) {
    al::setQuat(self, *reinterpret_cast<const sead::Quat<float> *>(reinterpret_cast<unsigned char *>(self) + 0x60));
    self->startClipped();
}
