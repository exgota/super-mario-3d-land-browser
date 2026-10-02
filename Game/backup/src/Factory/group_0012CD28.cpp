namespace {
extern "C" void fn_0012CEB0(void *);
extern "C" void fn_0012CD40(void *);
extern "C" void fn_0012EBB8(void *);
extern "C" void fn_002535A0(void *);
extern "C" void fn_00274930(void *);
extern "C" void fn_00274778(void *);
extern "C" void fn_00375774(void *);
}

namespace al {
class LiveActor { public: void makeActorAppeared(); };
class ByamlIter {};
class Camera { public: void load(const ByamlIter *); };
class NerveExecutor { public: void updateNerve(); };
}

extern "C" void fn_0012CD28(void *self) { fn_0012CEB0(self); fn_0012CD40(self); }
extern "C" void fn_0012EBA0(al::LiveActor *self) { self->makeActorAppeared(); fn_0012EBB8(self); }
extern "C" void fn_00253588(al::Camera *self, const al::ByamlIter *iter) { self->load(iter); fn_002535A0(self); }
extern "C" void fn_00274760(void *self) { fn_00274930(self); fn_00274778(self); }
extern "C" void fn_0037575C(al::NerveExecutor *self) { self->updateNerve(); fn_00375774(self); }
