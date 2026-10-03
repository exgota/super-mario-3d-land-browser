namespace {
struct Actor;
struct ActorVTable {
    void (*slot0)(Actor*);
    void (*slot1)(Actor*);
    void (*slot2)(Actor*);
    void (*appear)(Actor*);
};
struct ActorList {
    int count;
    int capacity;
    Actor** actors;
    Actor* get(int index) const;
    int size() const;
};
struct Actor {
    ActorVTable* vtable;
    char padding04[0x50];
    volatile signed char active;
    char padding55[0x1f];
    ActorList* volatile children;
    static signed char isActive(const Actor*);
};
}

extern "C" void _ZN2al11startActionEPNS_9LiveActorEPKc(Actor*, const char*);
extern "C" bool _ZN2al13isEqualStringEPKcS1_(const char*, const char*);
extern "C" Actor* fn_0024CC94(Actor*, const char*);

extern "C" void fn_00153868(Actor* actor, const char* action, Actor* child, bool body) {
    _ZN2al11startActionEPNS_9LiveActorEPKc(actor, action);
    if (!child) {
        int index = 0;
        ActorList* list;
        goto condition;
    loop:
        {
            Actor* candidate = list->get(index);
            signed char (*getState)(const Actor*) = &Actor::isActive;
            if (!getState(candidate)) {
                ++index;
                goto condition;
            }
            candidate->vtable->appear(child = candidate);
            goto found;
        }
    condition:
        list = actor->children;
        if (list->size() > index)
            goto loop;
    }
found:
    _ZN2al11startActionEPNS_9LiveActorEPKc(child, action);
    if (body) {
        if (_ZN2al13isEqualStringEPKcS1_(action, "DemoAppearTag")) {
            _ZN2al11startActionEPNS_9LiveActorEPKc(fn_0024CC94(child, "Body"), "DemoAppear");
        } else {
            _ZN2al11startActionEPNS_9LiveActorEPKc(fn_0024CC94(child, "Body"), action);
        }
    }
}

namespace {
Actor* ActorList::get(int index) const {
    return static_cast<unsigned>(index) < static_cast<unsigned>(count) ? actors[index] : 0;
}
int ActorList::size() const { return count; }
signed char Actor::isActive(const Actor* actor) { return actor->active; }
}
