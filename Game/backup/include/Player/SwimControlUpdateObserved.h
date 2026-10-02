#pragma once

#include <math/seadVector.h>
#include <prim/seadSafeString.h>

// Address-qualified observed views, not declarations of the original classes.
// The public PlayerProperty, IUsePlayerAnimator and graph-builder types remain
// unchanged. See project/evidence/swim-control-update-abi.md.
namespace swim174024
{
typedef sead::Vector3f Vec3;
struct PropertyView
{
    Vec3 translation;                       // +00
    Vec3 front;                             // +0C
    Vec3 up;                                // +18
    Vec3 velocity;                          // +24, observed role
    unsigned char unrecovered30[0x3C];
    Vec3 turnAxis;                          // +6C
};

struct InputView;
struct InputSlots
{
    void* unrecovered00[5];
    float (*axis14)(InputView*);
    float (*axis18)(InputView*);
    void* unrecovered1C[12];
    bool (*paddle4C)(InputView*);
    bool (*swim50)(InputView*);
};
struct InputView { InputSlots* slots; };

struct AnimatorView;
struct AnimatorSlots
{
    void* unrecovered00[2];
    void (*name08)(AnimatorView*, const sead::SafeString&);
    void* unrecovered0C[3];
    bool (*name18)(AnimatorView*, const sead::SafeString&);
    void* unrecovered1C[4];
    void (*operation2C)(AnimatorView*);
    bool (*query30)(AnimatorView*);
    bool (*name34)(AnimatorView*, const sead::SafeString&);
};
struct AnimatorView { AnimatorSlots* slots; };

struct ConfigurationView;
struct ConfigurationSlots
{
    void* unrecovered000[0x354 / 4];
    float (*horizontal354)(ConfigurationView*);
    float (*vertical358)(ConfigurationView*);
    float (*paddleAcceleration35C)(ConfigurationView*);
    float (*paddleMaximum360)(ConfigurationView*);
    int (*paddleFrames364)(ConfigurationView*);
    float (*swimAcceleration368)(ConfigurationView*);
    float (*swimSpeed36C)(ConfigurationView*);
    float (*swimDamping370)(ConfigurationView*);
    float (*idleDamping374)(ConfigurationView*);
    float (*lateralDamping378)(ConfigurationView*);
};
struct ConfigurationView { ConfigurationSlots* slots; };

struct ContextView
{
    PropertyView* property;                 // +00
    AnimatorView* animator;                 // +04
    void* unrecovered08[3];
    InputView* input;                       // +14
};
struct ActionView
{
    void* unrecoveredVtable;
    ContextView* context;                   // +04
    int paddleFrames;                       // +08
};
}

extern "C" void fn_00174024(swim174024::ActionView*);
