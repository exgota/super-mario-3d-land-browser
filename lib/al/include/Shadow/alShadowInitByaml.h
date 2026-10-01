#pragma once

namespace al {
class ShadowKeeper;
class LiveActor;
class ActorInitInfo;
class ByamlIter;
}

// Neutral identity for the observed four-argument entry, not an original name.
extern "C" bool fn_001C1064( al::ShadowKeeper*, al::LiveActor*,
                            const al::ActorInitInfo&, const al::ByamlIter& );
