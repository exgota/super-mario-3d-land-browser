#include <LiveActor/alLiveActor.h>

// Match the signed byte access used by the timer's actor-flag predicate.
extern "C" signed char readActorTimerDeadFlag(const al::LiveActor* actor) {
    return reinterpret_cast<const signed char&>(actor->getLiveActorFlag().isDead);
}
