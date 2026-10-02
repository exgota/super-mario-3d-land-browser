namespace al {
class LiveActor;
void invalidateClipping(LiveActor*);
class Scene {
public:
    void appear();
};
}

namespace rp {
void getPlayerPos();
}

extern "C" void fn_0026CCD0();
extern "C" void fn_002684B8();
extern "C" void fn_00272554();
extern "C" void fn_0029604C();
extern "C" void fn_0020540C();

extern "C" void fn_002E3648(void* p) { operator delete(p); }
extern "C" void fn_002F87D4(al::LiveActor* self) { al::invalidateClipping(self); }
extern "C" void fn_0031B340(al::Scene* self) { self->appear(); }
extern "C" void fn_0034E0D8() { fn_0026CCD0(); }
extern "C" void fn_0034E0DC() { rp::getPlayerPos(); }
extern "C" void fn_0034E0E0() { fn_002684B8(); }
extern "C" void fn_0034E0E4() { fn_00272554(); }
extern "C" void fn_00391030() { fn_0029604C(); }
extern "C" void fn_0039A9A0() { fn_0020540C(); }
extern "C" void fn_0039A9D4() { fn_0020540C(); }
