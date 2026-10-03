namespace {
struct Sequence;
struct SequenceVTable {
    void (*slot0)(Sequence*);
    void (*slot1)(Sequence*);
    void (*slot2)(Sequence*);
    void (*begin)(Sequence*);
    void (*slot4)(Sequence*);
    void (*slot5)(Sequence*);
    void (*update)(Sequence*);
};
struct Sequence {
    SequenceVTable* vtable;
    unsigned char padding[0x2c];
    bool active;
};
struct Display {
    unsigned char padding[0x54];
    Sequence* sequence;
    void setSequence(Sequence* value) { sequence = value; }
};
struct Owner {
    unsigned char padding0[0xc];
    Display* display;
    unsigned char padding10[8];
    Sequence* sequence;
    unsigned char padding1c[4];
    void* actor;
    void* sound;
};
struct Context { Owner* owner; };
struct Nerve {};
}
extern "C" {
bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Owner*);
bool _ZN2al6isStepEPNS_9IUseNerveEi(Owner*, int);
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Owner*, const Nerve*);
void fn_0018659C(void*);
void fn_00244DA8(void*, int);
void fn_0025276C(Display*, Sequence*);
void fn_00257060(Display*);
void fn_00257A3C(const char*, int);
int fn_00326678(Sequence*);
extern const char dat_003AE67C[];
extern const char dat_003AE694[];
extern const Nerve dat_003F0120;

void fn_00366D0C(void*, Context* context) {
    Owner* owner = context->owner;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(owner)) {
        owner->sequence->vtable->begin(owner->sequence);
        owner->display->setSequence(owner->sequence);
        fn_00257A3C(dat_003AE67C, 0);
    }
    if (_ZN2al6isStepEPNS_9IUseNerveEi(owner, fn_00326678(owner->sequence))) {
        fn_0018659C(owner->actor);
        fn_00244DA8(owner->sound, -1);
    }
    if (_ZN2al6isStepEPNS_9IUseNerveEi(owner, fn_00326678(owner->sequence) + 7)) {
        fn_00257A3C(dat_003AE694, 5);
    }
    owner->sequence->vtable->update(owner->sequence);
    fn_0025276C(owner->display, owner->sequence);
    if (!owner->sequence->active) {
        fn_00257060(owner->display);
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(owner, &dat_003F0120);
    }
}
}
