namespace {
struct ActorInitInfo;
struct FactoryContext;
struct ActorStorage { char storage[0x44]; };
}

extern "C" bool _ZN2al12isObjectNameERKNS_13ActorInitInfoEPKc(const ActorInitInfo&, const char*);
extern "C" bool _ZN2al16tryGetObjectNameEPPKcRKNS_13ActorInitInfoE(const char**, const ActorInitInfo&);
extern "C" ActorStorage* fn_0016428C(ActorStorage*, FactoryContext*, const ActorInitInfo&);
extern "C" ActorStorage* fn_00164E20(ActorStorage*, FactoryContext*, const ActorInitInfo&);
extern "C" ActorStorage* fn_00165A0C(ActorStorage*, FactoryContext*, const ActorInitInfo&);

extern "C" ActorStorage* fn_00153554(FactoryContext* context, const ActorInitInfo& info) {
    if (_ZN2al12isObjectNameERKNS_13ActorInitInfoEPKc(info, "Punpun")) {
        ActorStorage* actor = new ActorStorage;
        if (actor) return fn_0016428C(actor, context, info);
        return actor;
    }
    if (_ZN2al12isObjectNameERKNS_13ActorInitInfoEPKc(info, "PunpunVs2")) {
        ActorStorage* actor = new ActorStorage;
        if (actor) return fn_00164E20(actor, context, info);
        return actor;
    }
    if (_ZN2al12isObjectNameERKNS_13ActorInitInfoEPKc(info, "PunpunVs4")) {
        ActorStorage* actor = new ActorStorage;
        if (actor) return fn_00165A0C(actor, context, info);
        return actor;
    }
    const char* name = "";
    _ZN2al16tryGetObjectNameEPPKcRKNS_13ActorInitInfoE(&name, info);
    return 0;
}
