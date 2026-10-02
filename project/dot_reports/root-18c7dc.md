# Result timer countdown: exact local proposal

Base: `eeaca9ad7a606674b428b29630da497031ff913d`.
Source revision: `f16e36bd6f1e71347fd5f8e9b87486b4e7f969e4`.
Branch: `dot/root-18c7dc`.
Root: `fn_0018C7DC`, interval `[0x0018C7DC, 0x0018C9E0)`, 516 bytes.
Preflight found the row unnamed/U, with no source definition or overlapping proposal.

`Game/backup/src/Layout/ResultTimerUpdate.cpp` reconstructs the countdown,
one-coin conversion every ten ticks, extra-life callback and sounds, alternating
coin flash, label updates and completion result. Semantic names are descriptive
inferences; the exported function keeps its established address identity.

## Canonical verification

Ran after committing the source, using unchanged project tools:

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python make.py eu
python tools/check.py fn_0018C7DC --object build/eu/obj/Game/backup/src/Layout/ResultTimerUpdate.o
```

The final normal build linked `RE-Pepper.axf` and exported `code.bin`, exit 0.
The final checker exited 0 and printed:

```text
M -> O: The complete source-generated function interval matches byte for byte.
```

Two meaningful source forms: the first produced a 516-byte minor mismatch;
the second separated the tick increment from its test and put the completion
sound in its conditional scope. That form matched. No padding, register,
volatile, assembly, compiler-flag or tool changes were used.

A later declaration-only ABI reconciliation changed `fn_0027109C` from `void`
to `void*`. The normal build and the same committed-source targeted checker were
rerun successfully at the source revision above; the 516-byte root remains exact.

The first compact builds failed on the existing lowercase `fn_00270ccc` import
after unnecessarily naming that scratch map row uppercase. Restoring all six
callee rows to their original unnamed state fixed linking without source changes.

## Enrollment and ABI provenance

Only name/enroll the existing root row as `fn_0018C7DC` for independent checking.
No new data row or callee name is required. Imports encode existing function rows
at 00138B68, 00260984, 00270CCC, 00270DB0, 0027109C and 00326DEC.
The sole external data literal is the existing SafeString vtable row 003D9C34
with its normal +8 address point, 003D9C3C. Strings remain local C++ literals.

The configured Game module selected ARMCC 4.1/791, unchanged normal flags and
source discovery. Existing clean `LayoutActor`, `IUseAudioKeeper` and
`SafeString` headers provide the observed +4 interface conversion and 8-byte
string object. The local callback uses the observed virtual slot +4. The local
layout exposes pointers at +10/+14/+18 and integers at +1C/+24/+2C; unknown
fields only account for observed offsets. No shared header was changed.

The exact `CounterCollectCoin::collect` root at 00187AB8 grounds the shared sound
import's `void*` return: it stores that handle and passes it to `fn_00278B98`.
This declaration now agrees with that call-site evidence; the timer discards
the returned handle. Its argument types and all call sites are unchanged.

Compiler object SHA-256:
`08efe0c3bd86ab67b15029fcda4430b8f93c9b64c024f3b441ee9939a749de97`.
The owner executable hash was verified as
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

The full map is restored. This branch commits source and this report only;
no map/rank/ledger/STATE/tool/config changes or game data are included.
No broad gate or replay was run. Integrator acceptance remains required.
