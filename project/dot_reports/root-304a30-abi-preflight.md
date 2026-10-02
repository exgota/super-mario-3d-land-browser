# Gorori message handler: shared helper prerequisite

- Frozen main `3d69bf00a769676c77f07465163def56a559b48f`; branch `dot/root-304a30`.
- Target `0x00304A30..0x00304C60`, 560 bytes, remains U.
- The existing Gorori class and accepted init at 305644 provide the intended coherent family. The target receives the actor, message and two sensors, checks actor state and calls 27A624 with that actor and both sensors at 304AD0.
- Accepted `Game/backup/src/Factory/group_00156F9C.cpp` declares `void fn_0027A624(Actor*, Sensor*, Sensor*)` using its anonymous-namespace types. Its local Actor has a different layout from the accepted Gorori class; these are not interchangeable C++ types.
- No current shared/public declaration for 27A624 was found. Adding a new al::LiveActor/al::HitSensor signature would add another conflicting declaration form.
- Shared helper repair remains integrator-owned. No Factory edits, alternate alias or placeholder-compatible signature was attempted.
- No source or header changed, so accepted Gorori init was not rebuilt or regraded. No canonical check or demotion claim.
- Existing data references are mapped; this note concerns the type prerequisite only.
