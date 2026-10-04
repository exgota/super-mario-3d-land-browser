#include <Nerve/alActorStateBase.h>

extern "C" void fn_0018B4CC(al::ActorStateBase*);
extern "C" void fn_00142424(al::LiveActor*);

extern "C" void fn_00189B44(al::ActorStateBase* self) {
    fn_0018B4CC(self);
    fn_00142424(self->getHost());
    self->setDead(true);
}
