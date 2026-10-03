namespace {
struct LiveActor;
struct Quaternion;
struct Output;
}

extern "C" const Quaternion* _ZN2al7getQuatEPKNS_9LiveActorE(const LiveActor* actor);
extern "C" void fn_0027894C(Output* output, const Quaternion* quaternion);
extern "C" void fn_002789C8(Output* output, const Quaternion* quaternion);

extern "C" void fn_0027892C(Output* output, const LiveActor* actor)
{
    return fn_0027894C(output, _ZN2al7getQuatEPKNS_9LiveActorE(actor));
}

extern "C" void fn_002789A8(Output* output, const LiveActor* actor)
{
    return fn_002789C8(output, _ZN2al7getQuatEPKNS_9LiveActorE(actor));
}
