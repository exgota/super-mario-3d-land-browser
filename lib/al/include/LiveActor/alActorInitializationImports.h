#pragma once

namespace al
{
class ActorInitInfo;
class IUseStageSwitch;
class LiveActor;
}

// Reconstructed neutral imports, grounded by original callers and callees.
// The stage-switch result is meaningful; int follows its accepted tail callee.
extern "C" bool fn_002794F8( int* out, const al::ActorInitInfo& info );
extern "C" int fn_00280538( al::IUseStageSwitch* receiver, const al::ActorInitInfo& info );
extern "C" bool fn_0027FAB8( al::LiveActor* actor );
