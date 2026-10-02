# Photo media-store recovery: 0x002FB2F4

- Base: `713975727c0447ea8cdea708a9ab13cc539a92d0`.
- Branch: `dot/root-2fb2f4`.
- Target: `[0x002FB2F4,0x002FB51C)`, 552 bytes, rank U, no literal pool.
- Result: blocked at ABI preflight; zero source forms and zero exact credit.
- Investigation window: 2026-10-02 22:25–22:29 UTC.

## Blocking accepted imports

The root calls `fn_0021D750` at `0x002FB468`, then `fn_002FF6B8` at
`0x002FB474`, on the same eight-byte local entry. Both callees are accepted
O rows. Their canonical definitions in
`Game/backup/src/Factory/group_0010CAB0.cpp` take `Pair*`, where `Pair` is
an anonymous-namespace type with two `const void*` members.

That private C++ type cannot be named from a new Photo translation unit.
A separate local `Pair` is a distinct type even with identical layout;
redeclaring these C-linkage imports with that type or with `void*` would
not preserve the accepted source contract. Function-pointer casts, copied
helper definitions, or bypassing the calls would not resolve this identity.

Prerequisite: the owning integration lane must provide a shared, explicit
contract for these two accepted helpers and recheck their canonical
objects. This lane does not edit accepted Factory sources or repair their
private types. No root source is submitted before that prerequisite.

## Independently checked module and behavior

Caller `0x002FAB44` invokes this root with the MediaIndex object and storage
selector. Constructor `0x002FBF44` establishes the 0xB0-byte index, two
0x48-byte stores at +0x08/+0x50, flags at +0x04 through +0x07, and an ordered
entry list at +0x98. Store constructor `0x002FF34C` and initialization
`0x002FACD4` independently confirm that arrangement. The constructor's
installed dispatch already has data row `0x003DACFC`; no row is invented.

The root loads both metadata stores, handles their nonzero status results,
rebuilds missing data, packs live metadata entries into the ordered list,
and clears the selected unavailable flag only after its final save succeeds.
The entry helpers encode storage, slot, extension, dimensions and timestamp.
The appropriate existing build module is Game/backup/src/Photo with
configured ARMCC 4.1/791. This is not an allocation routine, and it contains
no direct allocator call or direct external data literal.

The accepted `fn_0021D970` declaration is compatible as `bool(void*)` and
needs no private-type repair. All other direct callees have existing function
rows. No missing-row blocker was found. The accepted `fn_0021DFB8` remains
`void*()`; this root does not call it or need a replacement declaration.

## Historical proposals and verification limits

Unmerged `dot/root-2faddc` at `8a08bab6a60cfd5012f053048153c439a2b7310a`
is the capped, nonexact MediaIndex refresh proposal and remains local. It
contains the same two private Pair imports. After integrator reconciliation,
its import types need rebasing. Its source-local duplicate Pair must be reconciled against
accepted definitions on this base before reuse; its historical report does
not establish cross-translation-unit C++ type identity.

Unmerged `dot/root-2ff3dc` at `9c4fd7e5108260d22ed35f2681c54750f55a48f4`
uses compatible observed MediaIndex/MediaStore layouts and a locally viewed
allocator wrapper. It supplies no shared Pair import contract for this root.
Neither older proposal was copied, rebased, built, or modified here.
This preflight makes no new failure claim about any accepted exact root.

The owner executable SHA-256 was verified as
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
No build, canonical check, replay, emulator, or preservation gate ran;
there is no candidate object or object provenance to claim. No source,
header, map, rank, boundary, tool, configuration or game-data file changed.
