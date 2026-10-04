#include <Player/ObservedPlayerEffectGroup.h>
#include <LiveActor/alActorInitUtil.h>

namespace {
// Constructor-owned storage. These uninitialized words reserve the complete
// observed allocation extents, not an alternate definition of al::LiveActor.
struct BreathBubbleStorage { unsigned int uninitialized[0x7c / 4]; };
struct WaterColumnStorage { unsigned int uninitialized[0x60 / 4]; };
typedef observed_containers::CountFirstPointerArray<al::LiveActor> ActorArray;
}

// Both original constructors return the receiver, whose LiveActor base is at 0.
extern "C" BreathBubbleStorage* fn_0019C4BC(BreathBubbleStorage*, const sead::SafeString&);
extern "C" WaterColumnStorage* fn_001957EC(WaterColumnStorage*, const sead::SafeString&);

namespace {
inline al::LiveActor* createBreathBubble() {
    BreathBubbleStorage* storage = new BreathBubbleStorage;
    if (storage)
        storage = fn_0019C4BC(storage, "EffectObjBreathBubble");
    return reinterpret_cast<al::LiveActor*>(storage);
}
inline al::LiveActor* createWaterColumn() {
    WaterColumnStorage* storage = new WaterColumnStorage;
    if (storage)
        storage = fn_001957EC(storage, "EffectObjWaterColumn");
    return reinterpret_cast<al::LiveActor*>(storage);
}
inline bool isFull(const ActorArray* array) {
    return array->count >= array->capacity;
}
inline void appendActor(ActorArray* array, al::LiveActor* actor) {
    int count = array->count;
    int capacity = array->capacity;
    if (count < capacity) {
        array->entries[count] = actor;
        ++array->count;
    }
}
}

extern "C" ObservedPlayerEffectGroup* fn_001B8574(
    ObservedPlayerEffectGroup* self, void* host,
    const al::ActorInitInfo& info, void* context) {
    self->breathObjects = new ActorArray;
    self->activeBreathObjects = new ActorArray;
    self->breathTimer = 0.0f;
    self->breathState = 0;
    self->waterObjects = new ActorArray;
    self->activeWaterObjects = new ActorArray;
    self->host = host;
    self->enabled = false;
    self->context = context;
    self->ripple = new al::EffectObj("EffectObjRipple");

    fn_0026AC60(self->breathObjects, 8, 0, 4);
    fn_0026AC60(self->activeBreathObjects, 8, 0, 4);
    while (!isFull(self->breathObjects)) {
        al::LiveActor* actor = createBreathBubble();
        al::initCreateActorNoPlacementInfo(actor, info);
        appendActor(self->breathObjects, actor);
    }

    fn_0026AC60(self->waterObjects, 4, 0, 4);
    fn_0026AC60(self->activeWaterObjects, 4, 0, 4);
    while (!isFull(self->waterObjects)) {
        al::LiveActor* actor = createWaterColumn();
        al::initCreateActorNoPlacementInfo(actor, info);
        appendActor(self->waterObjects, actor);
    }

    al::EffectObjFunction::initActorEffectObj(self->ripple, info, "EffectObjRipple");
    self->ripple->makeActorDead();
    return self;
}
