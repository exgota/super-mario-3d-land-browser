namespace {
struct LiveActor;
struct LayoutActor;
struct Scene;
struct NerveExecutor;
struct IUseNerve;
struct Nerve;
}

extern "C" void _ZN2al9LiveActor6appearEv(LiveActor*);
extern "C" void _ZN2al11LayoutActor6appearEv(LayoutActor*);
extern "C" void fn_0024ED7C(IUseNerve*);
extern "C" void fn_0026800C(IUseNerve*);
extern "C" void _ZN2al14onDrawClippingEPNS_9LiveActorE(LiveActor*);
extern "C" void _ZN2al5Scene6appearEv(Scene*);
extern "C" void _ZN2al10offCollideEPNS_9LiveActorE(LiveActor*);
extern "C" void _ZN2al9LiveActor17makeActorAppearedEv(LiveActor*);
extern "C" void fn_00192B48(IUseNerve*);
extern "C" void _ZN2al13NerveExecutor11updateNerveEv(NerveExecutor*);
extern "C" void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(IUseNerve*, const Nerve*);
extern "C" bool _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(const IUseNerve*, const Nerve*);

extern "C" Nerve dat_003F15A4;
extern "C" Nerve dat_003F17B0;
extern "C" Nerve dat_003F1800;
extern "C" Nerve dat_003F112C;
extern "C" Nerve dat_003F17F8;
extern "C" Nerve dat_003F3238;
extern "C" Nerve dat_003F324C;
extern "C" Nerve dat_003F1B74;
extern "C" Nerve dat_003F3260;
extern "C" Nerve dat_003F2F60;
extern "C" Nerve dat_003F2770;
extern "C" Nerve dat_003F1008;
extern "C" Nerve dat_003F347C;
extern "C" Nerve dat_003F3034;
extern "C" Nerve dat_003F3274;
extern "C" Nerve dat_003F11D4;
extern "C" Nerve dat_003F11C8;
extern "C" Nerve dat_003F17D4;
extern "C" Nerve dat_003F1E58;
extern "C" Nerve dat_003F1B54;
extern "C" Nerve dat_003F0F48;
extern "C" Nerve dat_003F0F40;
extern "C" Nerve dat_003F2F88;
extern "C" Nerve dat_003E2A98;
extern "C" Nerve dat_003F2F78;

class AssistItem {
public:
    void appear();
};

void AssistItem::appear()
{
    _ZN2al9LiveActor6appearEv(reinterpret_cast<LiveActor*>(this));
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(this), &dat_003F15A4);
}

extern "C" void fn_0012B930(LayoutActor* self)
{
    _ZN2al11LayoutActor6appearEv(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F17B0);
}

extern "C" void fn_0012B9E4(LayoutActor* self)
{
    _ZN2al11LayoutActor6appearEv(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F1800);
}

extern "C" void fn_0012CC8C(IUseNerve* self)
{
    fn_0024ED7C(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F112C);
}

extern "C" void fn_001368F8(LayoutActor* self)
{
    _ZN2al11LayoutActor6appearEv(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F17F8);
}

extern "C" void fn_0013E558(LayoutActor* self)
{
    _ZN2al11LayoutActor6appearEv(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F3238);
}

extern "C" void fn_0013E694(LayoutActor* self)
{
    _ZN2al11LayoutActor6appearEv(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F324C);
}

extern "C" void fn_00140B34(IUseNerve* self)
{
    fn_0026800C(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F1B74);
}

extern "C" void fn_001440B0(LayoutActor* self)
{
    _ZN2al11LayoutActor6appearEv(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F3260);
}

extern "C" void fn_00156BBC(LiveActor* self)
{
    _ZN2al14onDrawClippingEPNS_9LiveActorE(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F2F60);
}

extern "C" void fn_0015C4E4(LiveActor* self)
{
    _ZN2al9LiveActor6appearEv(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F2770);
}

extern "C" void fn_00160494(Scene* self)
{
    _ZN2al5Scene6appearEv(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F1008);
}

extern "C" void fn_00166498(LiveActor* self)
{
    _ZN2al10offCollideEPNS_9LiveActorE(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F347C);
}

extern "C" void fn_00170A2C(LiveActor* self)
{
    _ZN2al9LiveActor17makeActorAppearedEv(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F3034);
}

extern "C" void fn_00184D88(LayoutActor* self)
{
    _ZN2al11LayoutActor6appearEv(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F3274);
}

extern "C" void fn_0018648C(LayoutActor* self)
{
    _ZN2al11LayoutActor6appearEv(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F11D4);
}

extern "C" void fn_00186630(LayoutActor* self)
{
    _ZN2al11LayoutActor6appearEv(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F11C8);
}

extern "C" void fn_00187CBC(LayoutActor* self)
{
    _ZN2al11LayoutActor6appearEv(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F17D4);
}

extern "C" void fn_00192C08(IUseNerve* self)
{
    fn_00192B48(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F1E58);
}

extern "C" void fn_00195578(IUseNerve* self)
{
    fn_0026800C(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F1B54);
}

extern "C" void fn_001957CC(LiveActor* self)
{
    _ZN2al9LiveActor6appearEv(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F0F48);
}

extern "C" void fn_0019C104(LiveActor* self)
{
    _ZN2al9LiveActor17makeActorAppearedEv(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F0F40);
}

extern "C" void fn_001A1DA4(LiveActor* self)
{
    _ZN2al10offCollideEPNS_9LiveActorE(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F2F88);
}

extern "C" bool fn_001A57F0(NerveExecutor* self)
{
    _ZN2al13NerveExecutor11updateNerveEv(self);
    return _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003E2A98);
}

extern "C" void fn_001A8A6C(LiveActor* self)
{
    _ZN2al14onDrawClippingEPNS_9LiveActorE(self);
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(reinterpret_cast<IUseNerve*>(self), &dat_003F2F78);
}
