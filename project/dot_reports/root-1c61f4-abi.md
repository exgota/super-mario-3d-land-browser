# root001C61F4: accepted callback ABI blocker

- Frozen base: `8ae3d9d55e639057950efb7ba3a0c7ccc29c5f65`.
- Branch: `dot/root-1c61f4`.
- Target: `fn_001C61F4`, `[0x001C61F4, 0x001C6410)`, 540 bytes, rank U.
- Result: preflight only; zero source forms, zero builds/checks, zero exact claims.
- Map SHA256 before/after: `ea6710bd82d8b137e6ba0c331b0967ff48eed8e4963f75127aec9762f839162e` (unchanged).
- Owner code SHA256: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

## Module and behavior

This is an al lighting director's area-ID mapping initialization; the exact original class name remains inferred.
Caller `0x002752FC` passes the object at `LiveActorKit + 0x18`, immediately after its model-name mapping initialization at `0x001C618C`.
Neighbor constructor `0x001C6410` initializes the same fields and two default lighting objects.
The target looks up the `LightArea` collection, allocates 8-byte entries, skips area Arg0 ID -1 and duplicate IDs, formats `LightAreaId %04d`, and resolves a name through the Byaml iterator at director +0x38.
The resolved name is checked against 0x128-byte lighting records at +0x08 before a pair is appended to the circular buffer at +0x28.
Unresolved names append a null name. A nonnull name absent from the lighting records skips that area.

## Blocking accepted declaration

The target's constructor callback literal at `0x001C63F8` names the existing function row `fn_0039E170` (`0x0039E170..0x0039E180`, rank O).
It is passed to `__aeabi_vec_ctor_nocookie_nodtor` with element stride 8 and the area count.
The target later compares each entry's first 4-byte field with an integer area ID and writes `{int areaId, const char* lightName}` into the same entry.
The callback clears both 4-byte fields. Thus an `AreaLightEntry*` constructor signature is supported by this caller.

Current accepted source `Game/backup/src/Factory/group_0010CAB0.cpp` defines `fn_0039E170(Pair*)` using an anonymous-namespace `Pair { const void* data; const void* root; }`.
There is no shared declaration of this Pair or callback.
A new locally declared integer/string entry would not be the same C++ type, even though the ARM layout is eight bytes.
Calling through an incompatible constructor pointer, introducing an unrelated `void*` signature, or renaming the accepted row to a new constructor would conceal the conflict.
No such workaround was made. Factory and all shared declarations remain unchanged.
The existing formatted-buffer import `fn_0028CB38` and existing aliases were also preserved.

## Required next step and limits

A separately owned declaration cleanup must reconcile the accepted callback with a shared, caller-supported entry type and keep its original exact bytes.
Only then should this root be reopened in `lib/al/src/Light/` with the normal ARMCC 4.1/791 project build and unchanged `tools/check.py --object`.
No missing data row was found: the only out-of-interval target literal refers to the installed function row above; both strings are inside this function's own interval.
No vtable extent, table owner or enclosing director size is claimed.
No source implementation, object, provenance record, map/rank/ledger/tool/config edit, disassembly or game data is included in this note-only proposal.
