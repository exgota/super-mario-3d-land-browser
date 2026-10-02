namespace {
struct Nerve;
struct ControlTarget;
struct StateController {
    unsigned char unknown[0xC];
    ControlTarget* target;
    bool enabled;
};
}

extern "C" {
bool _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(const StateController*, const Nerve*);
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(StateController*, const Nerve*);
bool fn_0025A7AC(ControlTarget*);
void fn_0025A750(ControlTarget*, bool);
bool fn_0025A744(ControlTarget*);
void fn_0014CC30(ControlTarget*);
void fn_0025A708(ControlTarget*);
void fn_0027455C(int);
void fn_0025A698(ControlTarget*);
extern Nerve dat_003F3328;
extern Nerve dat_003F3330;
extern Nerve dat_003F332C;
extern Nerve dat_003F3324;
extern Nerve dat_003F3334;
extern Nerve dat_003F3320;
extern Nerve dat_003F3350;
extern Nerve dat_003F3358;
extern Nerve dat_003F3354;
extern Nerve dat_003F3348;
extern Nerve dat_003F335C;
extern Nerve dat_003F3344;
}

extern "C" void fn_0016EB68(StateController* self) {
    bool active = (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F3328)
                || _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F3330))
               ? fn_0025A7AC(self->target) : false;
    fn_0025A750(self->target, active);
    if (fn_0025A744(self->target)
        && (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F3328)
         || _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F332C)
         || _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F3324)
         || _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F3334))) {
        self->enabled = false;
        fn_0014CC30(self->target);
        fn_0025A708(self->target);
        fn_0027455C(3);
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F3320);
    }
    if (self->enabled)
        fn_0025A698(self->target);
}

extern "C" void fn_0016EFB4(StateController* self) {
    bool active = (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F3350)
                || _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F3358))
               ? fn_0025A7AC(self->target) : false;
    fn_0025A750(self->target, active);
    if (fn_0025A744(self->target)
        && (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F3350)
         || _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F3354)
         || _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F3348)
         || _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F335C))) {
        self->enabled = false;
        fn_0014CC30(self->target);
        fn_0025A708(self->target);
        fn_0027455C(3);
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F3344);
    }
    if (self->enabled)
        fn_0025A698(self->target);
}
