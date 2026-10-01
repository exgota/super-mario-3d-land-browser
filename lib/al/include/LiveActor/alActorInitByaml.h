#pragma once

#include <prim/seadSafeString.h>

namespace al { class LiveActor; class ActorInitInfo; }

// Neutral identity for the five-argument implementation called by the verified
// initActor and initActorWithArchiveName wrappers. The original name is unknown.
extern "C" void fn_002417E8( al::LiveActor* actor, const al::ActorInitInfo& info,
        const sead::SafeString& objectName, const sead::SafeString& archivePath,
        const char* suffix );
