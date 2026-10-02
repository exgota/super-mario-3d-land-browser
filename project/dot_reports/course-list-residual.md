# CourseList init: exact residual repair on current main

Base: `8991bec8cb27ff1a2dac0003f059d270a0b17493`. Branch: `dot/course-list-residual`. Work began 2026-10-02 01:31:23 UTC. The original packet and graded Pro response remain unchanged; this is a separate executed follow-up.

The final proposal changes only `Game/backup/src/System/CourseList.cpp`. It proposes the complete 408-byte `CourseList::init` interval `0x0016A11C..0x0016A2B4`, comprising 396 instruction bytes and 12 literal bytes. The unchanged project checker reports an exact match. Root intake remains required before this proposal contributes to main's accepted totals. World's separate 700-byte target is not proposed as exact.

## Faithful Pro form 3 reproduction

The exact C++ tokens from ranked form 3 in `project/pro_responses/0016A11C.md` were substituted for the List constructor and init in current main. Header, named string sections, accepted unsigned predicate, current string classes, flags and metadata were retained. The source was committed as `79b9596b3667c255329af233e597666b503fdd8d` before the ordinary project build.

The canonical check reported `M -> m: The linked candidate differs from the unchanged original interval.` Its complete section is 408 bytes and the canonical original-address comparison has exactly 11 differing bytes, reproducing the recorded Pro grade. Unrelocated section SHA-256: `b89a1d3dcd2ead31c20ac1a0667ec8eddcdcea76b47048c9a505e78900920a12`. Linked section SHA-256: `34e8ec0e00143cd0d97dd54f00defd6cc221862b29553b0d7acb6224a055436a`.

All differences are confined to four instructions immediately after the world-array allocation:

| Offset | Original | Reproduced form 3 |
|---|---|---|
| +0xA4 | `str r0, [r5]` | `ldr r1, [r5, #4]` |
| +0xA8 | `ldr r0, [r5, #4]` | `mov r4, #0` |
| +0xAC | `mov r4, #0` | `str r0, [r5]` |
| +0xB0 | `cmp r0, #0` | `cmp r1, #0` |

Every instruction outside that window and all three pool words agree. In particular, both allocator argument copies, allocation/null control flow, the r4/r5/r6/r7/r8 lifetimes, the root/world/child stack slots, the counting call boundary and the compiler's existing nop are already correct. The old packet's missing allocation copies are not the current residual mechanism.

## One grounded new form

The guarded do/while in the List constructor introduces an explicit pre-loop count test and an index lifetime starting before that test. The new form restores the ordinary structured fill loop, while retaining form 3's explicit inline constructor, early returns and init shared-exit control flow:

```cpp
mWorlds = new World*[ mNumWorlds ];
for ( int i = 0; i < mNumWorlds; i++ )
{
        al::ByamlIter curWorld;
        if ( worlds.tryGetIterByIndex( &curWorld, i ) )
                mWorlds[ i ] = new World( &curWorld );
        else
                mWorlds[ i ] = nullptr;
}
```

This hypothesis changes the loop-entry lifetime/control-flow lowering, rather than adding an allocator argument or a memory barrier. It retains allocation for any nonzero count and signed-positive iteration. It restored the original pointer store before count reload and comparison in r0. Committed `49b81a695686a60f35fd6911a86bc00198626875`, built normally, it produced `m -> O: The complete source-generated function interval matches byte for byte.` This was the first new form after reproduction: zero unsuccessful new forms and no cosmetic search. The final whitespace formatting was committed as `17321d85976b8d6e27abedcfcc121c7c129b1b05` and rebuilt normally.

Final source SHA-256: `7dce8815207a9e8b7b1214c2c6995ae5cdaec00ae2749259eb58ea5b5152465a`. Header remains `57cc60910de32098b88364e84c7ca54824d043d4aea4984bf42f266e45d67642`; CourseListCourse.cpp remains `fdbd40aed6e474f098167648f61d34e16c8e3c834ff9ff21b80eb9ea03005b89`. No shared header changed.

## Canonical validation

Only ARMCC 4.1 build 791, the project's game compiler, is required and was used. Compiler SHA-256 is `d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d`. Approved local compiler/venv and private EU data were symlinked; no objects or archives were shared. EU executable SHA-256 remains `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

The ordinary full build of reproduced source links and exports. A clean `make.py eu -ca` at the exact structured-loop commit also links and exports. The final formatted source rebuild links and exports. Its object SHA-256 is `78ae5a1f6b94fbf8dd1a423165a636beb6a092b4fc4ccac0e166dca377c9ea44`; unrelocated init section SHA-256 is `868b680b5a662d42aaa68dea19f92e5fee4ffed3284e1c560b9f823e4854a0ba`. The final linked 408-byte section equals the original hash `89ba8b60c76bb48cd4b9fb48ab852426f6a011005f2ba7d5481b86a08e0a9f8a`, with zero differing bytes.

The exact executed build/check commands were:

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python make.py eu
python tools/check.py _ZN10CourseList4initEPKN2al8ResourceE --object build/eu/obj/Game/backup/src/System/CourseList.o
python tools/low/checkExactBytes.py _ZN10CourseList4initEPKN2al8ResourceE build/eu/obj/Game/backup/src/System/CourseList.o
```

For the clean build, the build command was `python make.py eu -ca`. Both checker files are unchanged: `tools/check.py` SHA-256 `e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317`; `tools/low/checkExactBytes.py` SHA-256 `aef81a3a21c49bb17b8a19f139d02607899257e04dbe461ece2992cc726fc2e2`.

The final source preserves all **717/717 prior accepted roots across all 736/736 actual canonical definitions**, with zero failures. The sequential unchanged-checker sweep ran from 2026-10-02T01:36:35.524060+00:00 through 2026-10-02T01:47:26.001625+00:00 and took 650.477502 seconds. After that sweep, the final formatted source passed the new root again: `M -> O: The complete source-generated function interval matches byte for byte.` The 408-byte root has zero differing bytes. The local map was restored byte-for-byte to base8991, SHA-256 `2dde00f86cc0bc6f174f3a281853a4faacea1f16b6b628d602bea687b4cdb805`. No target name, Type, interval, pool extent or other metadata was altered.

The preservation manifest starts from the byte-for-byte original base map saved before any check. For each of its 717 O function symbols, it scans every project object with a provenance record whose source lies under Game/ or lib/, and collects every actual STT_FUNC definition. Generated split/stubs.c definitions are excluded. Multiple providers are all checked; this yields 736 definitions. Each is passed separately and sequentially to the unchanged `tools/check.py SYMBOL --object OBJECT`; success requires the literal `O -> O: The complete source-generated function interval matches byte for byte.` No prior snapshot with a smaller root set supplies this claim.

The executed checking loop, using the frozen manifest, was:

```python
import json, subprocess, sys
from pathlib import Path
manifest = json.loads(Path("build/course_residual/preservation-manifest.json").read_text())
for symbol, objects in manifest["objects"].items():
    for obj in objects:
        result = subprocess.run(
            [sys.executable, "tools/check.py", symbol, "--object", obj],
            text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        exact = result.returncode == 0 and (
            "O -> O: The complete source-generated function interval matches byte for byte."
            in result.stdout)
        # The execution recorded the result, full output and elapsed time for
        # every call in preservation-results.json and continued after errors.
```


## Source and helper closure

The only changed allocated section between the reproduced form 3 object and final object is init. List is completely source-inlined; neither List C1 nor C2 has a residual definition or import. No invented helper address, constructor alias, class layout, allocator overload, flag or source-level padding is used. All declarations and all definitions of Course, World, CourseList constructor, rp::getCourseList, and the emitted SafeString destructor/assureTermination leaves remain intact. The accepted stage predicate stays external in CourseListCourse.cpp.

The checker resolves the existing imports only: scalar/array nothrow new; Resource::getByml; ByamlIter data/default constructors, tryGetIterByKey, tryGetIterByIndex and getSize; the mapped World constructor; the accepted Course::isCourseTypeStage predicate; the Worlds and CourseList data sections; and the existing SafeString vtable address point. No unresolved helper closure remains. World is an existing mapped callee and its unmatched body does not become accepted merely because its caller matches.

All 15 compiled string sections equal their original intervals byte for byte, totaling 212 bytes from 0x003A2884 through 0x003A2958. No data table, section identity or boundary changed. The existing SafeString layers remain unchanged. The report's acceptance scope is one root, 408 bytes; already accepted Course/String bodies add no new credit.

No emulator or native functional comparison was run for this repair. The result is complete ARM function-byte equality under the approved build and existing callee identities. It preserves retail's null-allocation and invalid/null-entry behavior, rather than introducing fault recovery. There is no expanded valid-input-domain, FPSCR-mode, gameplay or port claim.

## Reproduction and submission files

Local-only evidence lives under `build/course_residual/`: faithful form source/object/provenance and exact JSON; structured-loop check/build logs; clean/final build logs; current-base preservation manifest and results; final exact JSON; source/helper/data inventory; and source hashes. These are generated diagnostics, not committed compiler outputs or game data. Original packet and Pro response hashes are recorded in final-source-hashes.json and both files are unchanged.

The source-only patch against base8991 is `/workspace/shared/course-list-residual-source.patch`, SHA-256 `1331143048867154b82e1cbe54074331e8beebc9ac1b5baf4d6e028fe102f41c`. Forward apply-check on pristine8991 and reverse apply-check on the submitted source both pass. The final family patch additionally contains only this report. Coordinator publication/intake is separate; no map, ledger, STATE, tools, configuration, binary or private data is included.

Report frozen 2026-10-02T01:48:30.795481+00:00. Assignment-to-report wall time is 1027.795481 seconds (17.129925 minutes), including reading, faithful reproduction, the one successful new form, ordinary/clean builds, all prior-definition checking, data/helper inventory and report preparation. The separately measured preservation sweep is 650.477502 seconds. Main has accepted zero new bytes from this unintegrated proposal at handoff, so main-accepted throughput is explicitly **0 bytes/hour** over this window. Local verification establishes a 408-byte exact proposal; it does not replace root intake or claim accepted throughput before integration.
