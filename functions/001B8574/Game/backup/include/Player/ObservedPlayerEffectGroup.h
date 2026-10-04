#pragma once

#include <Container/ObservedCountFirstPointerArray.h>
#include <LiveActor/alLiveActor.h>
#include <Npc/alEffectObj.h>

// Observed receiver for 001B8574; its original class name is unconfirmed.
struct ObservedPlayerEffectGroup {
    observed_containers::CountFirstPointerArray<al::LiveActor>* breathObjects;
    observed_containers::CountFirstPointerArray<al::LiveActor>* activeBreathObjects;
    float breathTimer;
    int breathState;
    observed_containers::CountFirstPointerArray<al::LiveActor>* waterObjects;
    observed_containers::CountFirstPointerArray<al::LiveActor>* activeWaterObjects;
    void* host;
    bool enabled;
    void* context;
    al::EffectObj* ripple;
};

static_assert(sizeof(ObservedPlayerEffectGroup) == 0x28, "observed receiver size");
static_assert(offsetof(ObservedPlayerEffectGroup, waterObjects) == 0x10, "water array");
static_assert(offsetof(ObservedPlayerEffectGroup, ripple) == 0x24, "ripple object");
