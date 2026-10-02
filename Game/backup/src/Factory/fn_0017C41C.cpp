namespace {
struct Actor {
    unsigned char padding[0x30];
    int state;
};
struct Controller;
struct Nerve;
}

extern "C" {
extern Nerve dat_003F18B8;
bool _ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(const Actor*, const Nerve*);
float fn_0032BD40(const Controller*);
int fn_0032BC28(const Controller*);
void fn_0027E6C8(Actor*, const char*, const char*, int);
void fn_002581A4(Actor*, float);

void fn_0017C41C(Actor* actor, const Controller* controller) {
    if (_ZN2al7isNerveEPKNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F18B8))
        return;
    float speed = fn_0032BD40(controller);
    int state = fn_0032BC28(controller);
    if (actor->state != state) {
        actor->state = state;
        switch (state) {
        case 0:
            fn_0027E6C8(actor, "Icon", "Wait", 0);
            break;
        case 1:
            fn_0027E6C8(actor, "Icon", "Backward", 0);
            break;
        case 2:
            fn_0027E6C8(actor, "Icon", "Forward", 0);
            break;
        }
    }
    fn_002581A4(actor, speed);
}
}
