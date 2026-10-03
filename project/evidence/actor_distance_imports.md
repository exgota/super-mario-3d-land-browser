# Actor distance import identity correction

The Codex driver owns this cleanup on `cleanup/actor-distance-imports`, frozen at integrator main `1c4fcc0e1e6bc02dda48210ac23d6a87f14e36e3`. Source commit `444b007a68bba2126e10aaf69b6c691a93c65de2` changes one accepted drawing translation unit, adds one declaration-only import header, and corrects its stale facts entry. No helper body, new hard target, map identity, rank or exact byte is proposed.

## Identity and contract

The original interval `0x0021E2D0..0x0021E338` is 104 bytes. It calls accepted `al::getTrans(const al::LiveActor*)` at `0x0028028C` for both incoming actor addresses, reads the three translation components, and compares squared separation with the square of the floating-point radius. It returns one for an ordered strict less-than result and zero otherwise. Equality and unordered comparisons fail. The radius is squared directly; there is no sign clamp or null guard. Neither this helper nor its getter writes either actor. The parent independently inspected this complete original body from the owned EU executable, whose SHA-256 remains `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

The accepted drawing routine at `0x0021E1D8` has a different alternate virtual-dispatch path. Its old private inline used the distance function's neutral address name despite having an integer index, drawing context and Vector4 argument. The original drawing routine never calls the distance helper. Its private helper is now `drawAlternate` with internal C++ linkage, the same inline body, parameters and call shape. No public emitted interface is removed.

`alActorDistanceImports.h` records a bounded neutral import taking two `const al::LiveActor*` values and a float, returning bool. Bool and const describe the observed predicate/read-only contract; the original return type, const qualification and pointer-versus-reference spelling remain unproved. The row is already available under the toolchain's neutral address symbol, so no map edit is needed. The header is included by the drawing file to compile the shared declaration and prevent reuse of its address name for the incompatible private helper. No current source caller of the actual distance predicate is added. The reported 572-byte target at `0x002F7298` remains report-only.

## Verification

The repository's normal ARMCC build completed successfully from the committed source. `python tools/check.py fn_0021E1D8 --object build/eu/obj/Game/backup/src/Factory/fn_0021E1D8.o` returned zero and reported the complete source-generated interval exact. All 248 bytes, including the eight-byte literal pool, remain accepted locally. The check took 0.400247 seconds. The scratch map was restored byte-for-byte to HEAD after checking. This is preservation of one prior exact function and zero new credit.

The rebuilt object defines only `fn_0021E1D8`, with no emitted private drawing helper and no defined or imported `fn_0021E2D0`. Its ordinary project object SHA-256 is `442fc41fb661c374e93c416a30b3e9b0548dc1831a5797ae8a0ec1c39d33bae0`. The new header reaches only this translation unit. The whole normal-object strong duplicate-definition audit reports zero. The obsolete drawing signature has zero occurrences in tracked Game/lib source; the sole neutral address declaration is the new actor-distance signature. Unrelated drawing imports and private layouts are unchanged and are not certified by this correction.

The integrator's complete previous-root regression gate remains required. This cleanup is deliberately held while the ready dot source family is pending, because current sync selection would run a cleanup before ordinary riders despite dot-first sorting.

Independent read-only review of the exact three source/facts hashes found zero source-level blockers. It confirmed the new header is optional for the local rename but acceptable as the separately owned shared contract. The parent retains it to publish the grounded import for future callers. The review report SHA-256 is `7f3e94972e0a386e642f6554c8f38353b5d1a1fba5104ee8d926d5bf0066c53c`. This review did not replace the parent build/check or the pending integrator gate.

## Final source hashes

- `Game/backup/src/Factory/fn_0021E1D8.cpp`: `5b871634f5522ada2ca24751efc4f2567f6ce0613b0414d37cd8008411a649fa`
- `lib/al/include/LiveActor/alActorDistanceImports.h`: `4dcd17e84ee4a5c41cc5b1c384e666275a342009fc656d239314659c6e5be0ac`
- `docs/facts/0021E1D8.md`: `51edee3be776104fbdd9b931489cbe4c148eba48a4eac52bac4a8b4b275cf3ba`
