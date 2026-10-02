# Swimming-control update proposal

**No exact-match claims.** `fn_00174024`, original00174024..00174828, is a
complete2,052-byte root including its internal literal island and tail strings.
The final committed, guarded C++ compiles to2,096bytes. The canonical checker
rejects its extent. This branch proposes useful NonMatching source and independent
ABI evidence; it adds zero accepted roots and zero accepted bytes.

## Ownership and base

Branch `dot/swim-control-update` is based on frozen
`de61f9bfed35d5dd22a4ffb24cc98a910ef0b7b0`. The fresh queue observation at08:19–08:20
UTC found main`269591a54f4bc63fca59383a81b08bd86071b620`, with the target stillU and
only two new Factory CPPs/map/ledger since the frozen base. All larger eligible
roots had existing owners; active0038F9F0 belongs to another lane. The public
resume instruction supersedes the stale planning pause in frozen STATE. Only
the integrator may move main, enroll ranks or write the ledger.

This proposal owns only the two new swimming files and these notes. It does not
change the pending `dot/player-action-builder-source` family, public player or
animator headers, map, ledger, STATE, compiler flags, tools or configurations.
`project/evidence/swim-control-update-abi.md` is the separate observed-view and
shared-import evidence. The original class name remains unknown.

## Source and attempt history

- `Game/backup/src/Player/SwimControlUpdate.cpp`, SHA256
  `482c3f5771512c42ebf4a1d1c94a1d8d1e687f35b7f8b78252ff33f60f8fe695`
- `Game/backup/include/Player/SwimControlUpdateObserved.h`, SHA256
  `3bd222318cffe7fbd151c28fbedb790d6c6649ea6cd2ca079a8c8ba5985482ef`

Three meaningful source forms were committed and kept in history:

1. `57bec9a`: full control flow and isolated observed layouts,2,100bytes; canonical
   extent mismatch
2. `1db8efe`: moved horizontal-rotation negation after the product,2,100bytes;
   the compiler produced the same problematic negative-constant operation.
   Replay of 560 cases exposed five negative-NaN-sign differences
3. `bdb31f8`: explicitly negates the rounded product's sign bit, preserving the
   observed VNMUL behavior under the fixed fast-float flags,2,096bytes;
   canonical extent mismatch. Expanded final replay passes all 562 cases

Source tuning stops at three forms, without a grounded further size/codegen
hypothesis. The final `NON_MATCHING` guard was present before the clean build.
No dummy arithmetic, assembly, byte stand-in, invented source helper address,
flag change, checker alteration or bound/type edit was used.

The initial tool invocation refused an unenrolled unnamed symbol. The first
pre-commit compile also had `inputs_stable:false`, and the checker correctly
refused that provenance. Both outputs remain in the local evidence history.
After commit, a normal project rebuild produced stable committed-source
provenance and every meaningful form was checked normally. Those infrastructure
refusals are not byte comparisons or extra source forms.

## Final build and canonical result

The final normal `python make.py eu -ca` clean build linked successfully, return0.
It ran in the08:35–08:37UTC observation window; this window includes polling and
is not a measured build duration. The final object SHA256 is
`fa4311d2ffd87463027c9d2888b09ac79ffee196c4e667cc110b188486fcff8e`.
Its stable provenance SHA256 is
`f3e64fdf8e720f470a55470318120cd418401f45d6d32e82552e8d612d41aa73`.

The unchanged checker was invoked on that normal project-built object:

```
python tools/check.py fn_00174024 --object build/eu/obj/Game/backup/src/Player/SwimControlUpdate.o
M -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

Only this checkout's original row was temporarily assigned its address-derived
symbol and scratchM rank, with all interval/type fields unchanged. Exact prior
map bytes were restored in a finally block. No scratch metadata is committed.
The checker returned1. It rejected the extent before reporting exactness; a
successful diagnostic link is not a canonical source-closure acceptance.
Final checker-output SHA256:
`45a54c1af5d739b44f5160edbcdf1fc69f44ef2fcbd9a80fcf97a4b1b6682ae0`.
Clean-build log SHA256:
`1fb60dc5f09491237f26604bc1eb53a538e44b14981b775e6743e865e67af8d3`.

## Bounded original-code replay

`project/evidence/swim-control-update-reproduction.md` contains the full
source-only diagnostic-link, fixture/replay and preservation recipes. The
fixture generator preserves exact IEEE input words and uses seed00174024.
Final run: 562 cases, 556 returns and 6 matched null-pointer faults, zero comparison
failures, 504,485 combined executed instructions in 6.407 seconds. All 497 executable
instructions of the original root were reached across the suite. Reaching every
instruction is not a proof of every path or input.

The original rotation, sine/cosine, normalization, cross/scale/add/sub and
00173F4C direction/speed helpers execute unchanged. Explicit external models
supply the configuration getter and the virtual provider/input/animator calls;
providers are constant and have no side effects, and animator calls are recorded
without running real animation machinery. The first discarded-result name34
call is retained. No concrete input/configuration class identity is asserted.

Each execution is bounded at 50,000 instructions. Comparisons cover all 64 KiB of
fixture memory, protected caller-stack bytes, ordered virtual events/helper calls,
FPSCR exception/mode bits, return/fault class and fault access/address/size.
SP and callee-saved core/VFP registers are verified on returns. Void r0 and FPSCR
condition flags are outside the contract. Null-pointer cases establish only
matched bounded faults; extreme signed counters describe this ARMCC build.
Arbitrary aliasing, mutable/reentrant provider behavior, unmodeled runtime state,
gameplay, rendering and full executable equality are not established.

Fixture JSON SHA256:
`9867d58ea08813feca5a140ae8be427d919bb4ae1cb9ca4df1f9269272e68cdd`.
Final replay report SHA256:
`2b6e02be65d6f7afa418480b1df1a6851131169fd3df7ebac20bf607e1017fe2`.

## Prior-root preservation

The pristine de61 baseline independently clean-built and passed 893 roots/all 913
actual canonical definitions, return0 and no failures, in341.266809 seconds
including 22.393520 seconds for the clean build. Its report SHA256 is
`3dc43ab15b453b9c79d328aadadd0d4ffb319b0b6d05c59b7dd722fd245f1970`.
Read-only verification confirmed the baseline's tracked source/map/config/tools
still equal de61. That baseline was neither changed nor rerun here.

This proposal audited every actual prior Game/lib C++ object:217 objects and410
committed input paths, with unchanged source/input hashes, compiler hashes,
normalized commands and stable provenance.216 objects have identical allocated
sections, relocations and function definitions. Raw object hashes differ because
build paths are embedded in nonallocated metadata; raw-file equality is not
claimed.

The remaining Factory object,`fn_00375D68.o`, has identical function definitions
and relocations but one different allocated RTTI string: ARMCC's anonymous
namespace token embeds a worktree-dependent hash (`1daf469e` versus`89b84edf`).
No production source was changed to remove it. Its fresh unchanged canonical
check returned0 and reported:

```
O -> O: The complete source-generated function interval matches byte for byte.
```

Thus this is exhaustive equivalence-backed preservation plus one fresh affected
root check, not a new full 913-check run. Four new weak SafeString destructor/
assureTermination aliases have allocated-section equivalents in prior provider
objects, including their zero-size destructor alias convention.
All 2,376 prior generated scaffold sections are identical. Six added sections are
address aliases for00173F4C,00174024,00258A54,0026E1DC,00270844 and00279ABC;
those remain placeholders and add no behavior or exact credit.

The initial exhaustive comparer stopped on the RTTI difference. A subsequent
local comparison was interrupted while its repeated ELF-section parsing ran;
an indexed parser completed the same checks in 7.544547 seconds. This was a change
to the disposable evidence recipe, not any project tool or acceptance oracle.
Preservation report SHA256:
`cfbcf0d8768e29802c23af192d11df4f99ebc3dd332cc3da9a8fa61f961033d0`.

## Tools and handoff limits

- Original EU code SHA256:`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`
- Original exheader SHA256:`94e61359c80498495dd77bb2df16f0def6fca94fca736dc8c3c929279b44d2c8`
- ARMCC4.1/791 SHA256:`d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d`
- Installed SDK compiler4.0/902 SHA256:`e6aca4ebc280a54065406bcc3a0ae3d9f4d0ac5a4032c3a193adb836c72e70fe`
- wibo SHA256:`aee836280b3d7c80c2031410219c907c9824c11be6b128d0744655de0f22280b`
- tools/check.py SHA256:`e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317`
- make.py SHA256:`1d5ba9628e78a4aaa23d6bbbea40131101a46ee41ca43e3df124eaa97d55241a`
- buildProvenance.py SHA256:`343d1a0954aba9a4805e71420be9a99d91185db4c8b329d1363d11efcee53529`
- Unicorn2.1.4 shared-library SHA256:`ddb196ec82b52e502c18e4a34478bf7b9f61c83c2ebaa95c74d8ded45a95da9c`

Build894 is absent and was not downloaded. There is no new compiler-discrimination
claim. Complete local logs, source-form snapshots, fixtures, diagnostic ELF,
comparison reports and their hashes remain in ignored`build/swim-control-update/`.
Only C++/header/notes are submitted. Original executable/exheader, compiler files
and generated binaries are never included in the patch. Local work began08:22UTC
and completed approximately 08:47 UTC on 2026-10-02; zero newly accepted bytes means
zero acceptance throughput. Only the integrator can turn a later exact checker
result into accepted credit.
