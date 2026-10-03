namespace {
struct NerveAction {
    const void* vtable;
    void* action;
};
struct NerveActionCollector;
}

extern "C" {
NerveActionCollector* _ZN15alNerveFunction20NerveActionCollectorC1Ev(NerveActionCollector*);
NerveAction* _ZN2al11NerveActionC2Ev(NerveAction*);

extern NerveAction dat_003F285C;
extern NerveAction dat_003F2864;
extern NerveAction dat_003F286C;
extern NerveAction dat_003F2874;
extern NerveAction dat_003F287C;
extern NerveAction dat_003F2884;
extern NerveAction dat_003F288C;
extern NerveAction dat_003F2894;
extern NerveAction dat_003F289C;

extern const unsigned dat_003BE210[];
extern const unsigned dat_003BE224[];
extern const unsigned dat_003BE238[];
extern const unsigned dat_003BE24C[];
extern const unsigned dat_003BE260[];
extern const unsigned dat_003BE274[];
extern const unsigned dat_003BE288[];
extern const unsigned dat_003BE29C[];
extern const unsigned dat_003BE2B0[];
}

extern "C" void fn_0038AE74() {
    _ZN15alNerveFunction20NerveActionCollectorC1Ev(reinterpret_cast<NerveActionCollector*>(0x0042FD1C));
    _ZN2al11NerveActionC2Ev(&dat_003F285C)->vtable = dat_003BE210;
    _ZN2al11NerveActionC2Ev(&dat_003F2864)->vtable = dat_003BE224;
    _ZN2al11NerveActionC2Ev(&dat_003F286C)->vtable = dat_003BE238;
    _ZN2al11NerveActionC2Ev(&dat_003F2874)->vtable = dat_003BE24C;
    _ZN2al11NerveActionC2Ev(&dat_003F287C)->vtable = dat_003BE260;
    _ZN2al11NerveActionC2Ev(&dat_003F2884)->vtable = dat_003BE274;
    _ZN2al11NerveActionC2Ev(&dat_003F288C)->vtable = dat_003BE288;
    _ZN2al11NerveActionC2Ev(&dat_003F2894)->vtable = dat_003BE29C;
    _ZN2al11NerveActionC2Ev(&dat_003F289C)->vtable = dat_003BE2B0;
}
