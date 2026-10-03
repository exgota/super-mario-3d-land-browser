# Player velocity steering shared vector prerequisite

Branch: dot/root-172c68. Refreshed main/base: cf2886000d0c49e12faddf4cd77a1d47372735c8.
Target: 00172C68–00172EA8, 576 bytes, U. Exact claims: none.

The target normalizes local three-float vectors through accepted fn_00279ABC at
00172CE4, 00172D30 and 00172DE0. It therefore has the same shared-type prerequisite
as [root-17231c-abi-preflight.md](root-17231c-abi-preflight.md) and
[root-160770-abi-preflight.md](root-160770-abi-preflight.md).
Factory group_002438D4.cpp owns that helper through an inaccessible anonymous Vec3.
A case-insensitive search of current Game/lib sources found no ordinary public
declaration usable by this target. No additional private type or alias is proposed.

New dependent-caller evidence:
- Game/Player ownership is strongly supported by the call at 00172DEC to the
  established PlayerProperty::setFrontVec(const sead::Vector3f&) interface.
- Argument r0 is PlayerProperty*; r1 supplies virtual direction callbacks at
  vtable offsets +8 (validity) and +12 (vector reference). Three float arguments
  arrive in s0–s2. The direct caller at 0019F57C lies in root 0019F43C.
- The target reads front +0x0C, up +0x18 and a projection normal +0x6C; it reads
  and finally writes velocity +0x24. These are three-float fields.
- Helpers 0027306C and 0026FFA0 split velocity relative to the normal. Valid input
  steers the front and blends tangential velocity; invalid input scales it by the
  third float argument. The normal component is then added back.
- Constants 0, 1 and 0.5 lie inside the existing target interval. There are no
  direct external data references and no missing-row claim.

Stop: reconcile the existing accepted helper's shared declaration through its
owning cleanup lane, then revisit this caller. Zero source forms were attempted.
No source/header changes, build, canonical check, replay or full-map audit ran.
The complete map remains unchanged. No mismatch, runtime failure or demotion is claimed.
