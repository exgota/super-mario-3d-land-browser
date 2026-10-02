namespace al {
struct IUseNerve {};
struct LiveActor : IUseNerve {};
struct Nerve {};

void startAction(LiveActor*, const char*);
void setNerve(IUseNerve*, const Nerve*);
}

extern "C" al::Nerve dat_003F2378;

extern "C" void fn_00321BEC(al::LiveActor* actor) {
    al::startAction(actor, "Land");
    al::setNerve(actor, &dat_003F2378);
}
