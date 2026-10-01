# MemorySystem scene resource heap

## Scope and status

Target: `al::MemorySystem::createSceneResourceHeap(const char*)`, existing unchanged interval `0x0024F344–0x0024F484` (320 bytes, including the 32-byte literal pool). Work started from main `a360142fbddd9ab875ab68314c55445ade05fd00`. The supplied EU executable independently hashes to `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

The source repairs the retained fragment and the parent's canonical checks have verified it as NonMatching. `NON_MATCHING` remains in place. The final candidate is rank `m`; no exact match is claimed. The first pass stops at eight candidates. Each candidate and its measured result is recorded below. No changes to tools, compiler flags, function boundaries, or the target executable are required.

## Source defects recovered

The heap-size local is unsigned, not signed: the target converts the float product with `vcvt.u32.f32`. Its initial value is always 8 MiB, including when a non-null stage name has no matching row or the table is empty. The former fragment left that path uninitialized.

The frame-heap helper at `0x002911E8` takes six arguments. The former five-argument declaration left its sixth parameter unspecified. The caller passes the output field at `this+0x0C`, size, name, null parent, lock enabled, and forward direction. The helper stores the resulting heap through the supplied field and clears flag bit `0x4` at heap offset `0x6C`. The precise semantic name of this flag is not asserted.

The ordinary C++ flow otherwise agrees: locate the table, construct a ByamlIter, scan its rows in order, compare the Stage string, fetch SceneResource with a zero float default, multiply by 1024 twice, convert to unsigned, and stop after the first matching stage. A matching row with a missing/invalid SceneResource therefore requests size zero. Failure to find the stage retains 8 MiB.

The existing `mSceneResourceHeap` field is already correct at offset `0x0C`. No MemorySystem or shared header edit is necessary.

## Independently established imports

- `0x00243260`: `al::findOrCreateResource(const sead::SafeString&)`, proposed symbol `_ZN2al20findOrCreateResourceERKN4sead14SafeStringBaseIcEE`. Independent `ActorFactory::ActorFactory` at `0x0026902C` calls it at `0x00269058` with `SystemData/CreatorClassNameTable`, then stores the returned resource. The wrapper obtains the resource subsystem via `0x00292D10`, loads its field at `+8`, and falls through into `0x0024327C`. That implementation first searches via `0x00290698`; a hit returns directly. A miss selects a resource category and transfers to creation at `0x00290B18`, whose allocation invokes the already named Resource constructor `0x00105420`. This supports the find-or-create identity independently of the heap target.
- `0x00290640`: `al::Resource::getByml(const sead::SafeString&) const`, proposed symbol `_ZNK2al8Resource7getBymlERKN4sead14SafeStringBaseIcEE`. Independent ActorFactory calls it at `0x0026908C` with `CreatorClassNameTable` on the resource above, and passes its result to the accepted ByamlIter constructor at `0x002905F0`. The callee itself formats `%s.byml` and asks its archive object for the resulting file. `CourseList::init(const al::Resource*)` independently calls it at `0x0016A164`.
- `0x0029101C`: `al::ByamlIter::tryGetStringByKey(const char**, const char*) const`, proposed symbol `_ZNK2al9ByamlIter17tryGetStringByKeyEPPKcS2_`. Its implementation calls the independently named `getByamlDataByKey` at `0x0028CAB4`, rejects tags other than the BYAML string type `0xA0`, obtains the string-table offset from header `+8`, resolves the indexed string through `0x0028CA30`, stores it through the output pointer, and returns true. The tiny `0x0028CA30` row remains unnamed; its string-table lookup interpretation is from its own implementation, not a previously accepted symbol.
- `0x002911E8`: existing unnamed map row used as `fn_002911e8`. Its arguments after name are `sead::Heap*`, `bool enableLock`, and `sead::Heap::HeapDirection`. The independent MemorySystem constructor passes its stationed heap as parent at `0x00101D5C` and `0x00101D78`; the independent course-select heap creator passes direction `-1` at `0x001C0F04` and `0x001C0F30`. In the helper, incoming argument 6 becomes register argument 4 of `0x00105EBC`; that routine uses it for signed allocation alignment and front/back header placement. Incoming argument 5 becomes the stack flag propagated into the base heap constructor `0x0028B6A4`, which makes heap flag bit 0 conditional and uses it to control critical-section locking. The existing map's much longer inferred `FrameHeap::tryCreate` prototype is not required or changed here.

Existing established imports used unchanged are ByamlIter constructors at `0x002905F0` and `0x002910E8`, getSize at `0x002910F8`, tryGetIterByIndex at `0x0029107C`, tryGetFloatByKey at `0x00278C30`, and char-pointer isEqualString at `0x00292308`.

## Data identities

The target uses external strings rather than compiler-local string pools. Each has its own already existing data row, so address-encoded external declarations require no boundary or content change:

| Address | Existing extent | Contents |
| --- | --- | --- |
| `0x003A2230` | `0x20` | `ObjectData/GameSystemDataTable` |
| `0x003A2220` | `0x10` | `HeapSizeDefine` |
| `0x003A2250` | `0x08` | `Stage` |
| `0x003A2258` | `0x10` | `SceneResource` |
| `0x003A2268` | `0x20` | `SceneHeapResource` |

The SafeString virtual table is the already named row at `0x003D9C34`; the constructed object uses its normal ABI address point at `+8`, `0x003D9C3C`. That same address point appears in independent ActorFactory construction and in the frame-heap helper. No virtual-table row repair is needed.

The float constants are ordinary C++ values zero and 1024. No instruction arrays, assembly, binary-derived object sections, or embedded retail function bytes were introduced.

## Checker record

Candidate 1 was committed as `cd918ea` and compiled by the parent's canonical build. The parent reports `tools/check.py --object` produced `U -> m`. The linked candidate is the correct 320 bytes and its entire 32-byte literal pool is exact. Its remaining differences concern register allocation, local stack allocation, and the merge of the calculated size into the final heap call. All import and provenance checks pass.

Candidate 2 extracts the size calculation into an ordinary inline C++ helper with an explicit default-size argument and early return on a match. The retail success path places the converted value directly in the final call's size register, while the no-match path copies a preserved default size. This is the grounded hypothesis for separating the calculation's result from the retained default; the canonical compiler/linker will decide whether the helper is inlined. The source remains NonMatching until strict verification.

Candidate 2, committed as `d587a52`, also returns `m -> m` in the parent's strict check. Its inline helper is compiler-inlined without a closure dependency. The 320-byte extent and literal pool remain exact. The float and string local stack slots now match, but the entry iterator and persistent registers differ; the compiler still coalesces the default and computed size into one persistent register.

Candidate 3 returns to a single function, with distinct default/computed-size variables and an explicit jump from a matching row to heap creation. Only the no-match path assigns the default to the result. This tests the retail success/no-match merge directly without introducing a runtime helper.

Candidate 3, committed as `a69ad8d`, returns `m -> M`: its section is 316 bytes instead of 320. The desired success path now places the converted size directly into argument register r1, but the compiler folds the preserved default into an immediate load on the no-match path and removes r8 from the saved register set. This does not reproduce retail's retained default register.

Candidate 4 restores candidate 2's inline helper and passes its return value directly as the frame-heap helper's second argument, without a named result local in the caller. This tests whether argument evaluation preserves the default while avoiding the result coalescing observed in candidate 2.

Candidate 4, included in `a9fce38`, produces the same linked function bytes as candidate 2. The refreshed strict-check candidate has the same 320-byte extent, exact pool, and differing register/entry-iterator allocation. Direct argument spelling therefore does not fix the merge.

Candidate 5 uses a normal inline try-get helper with an output pointer and boolean success. The caller initializes the size to 8 MiB, and the helper updates it only after a matching row. The boolean result is intentionally unused because failure leaves the initialized default intact. This is an output-parameter dataflow hypothesis, not an asserted original helper identity.

Candidate 5, committed as `2e0a6c1`, receives `m -> m` after a forced fresh canonical compile. It retains the exact extent/pool but still coalesces the default/result in r5 and puts the entry iterator at stack +0x20 instead of +0x10. Output-parameter spelling does not resolve this.

Candidate 6 moves the null-stage guard to the caller. A non-null lookup helper takes the existing default as an argument and returns either it or the newly calculated size. This tests a nested caller/helper result merge rather than placing the guard within the lookup helper.

Candidate 6, committed as `99f4934`, receives `m -> M`: its compiler section is 324 bytes. The success path now sends the converted value directly to r1 and the no-match path separately transfers the saved default, as retail does. However, the caller initializes r1 and transfers it to the helper's persistent r5 after the null guard, adding one instruction and changing the remaining allocation.

Candidate 7 places an early null return inside the inline helper, before table construction, with a separate no-match return after the loop. This keeps two default exits without the extra caller/helper transfer observed in candidate 6. It is the final control-flow-boundary test before the eight-candidate first-pass cap.

Candidate 7, committed as `872db73`, receives `M -> m`. It generates the same full-extent allocation pattern as candidate 2, including the correct literal pool. The early-null-return spelling is not a solution.

Candidate 8 is the final first-pass candidate. It swaps the source-only inline helper's argument order to `(defaultSize, stageName)`, testing whether the inlined argument placement drives the persistent-register choices. This signature is not proposed as an independently identified retail import. If the result remains nonmatching, this lane stops compilation at the eight-candidate cap and retains a complete guarded implementation.

Candidate 8, committed as `684e238`, was canonically rebuilt and leaves the map rank `m`. The linked function is identical to candidate 7: full 320-byte SHA256 `5586aa45c1992f30b3dfbdcd191685942932201b30652d66601ff203d3e624fd`. The final retained source is this guarded, semantically recovered candidate. The Capstone/byte diagnostic counts 18 differing instruction words in the 288-byte code region and an exact 32-byte literal pool; this diagnostic count is not a matching grade. No more variants were compiled after the cap.

| Candidate | Committed source | Compiled extent | Strict outcome |
| --- | --- | --- | --- |
| 1 | `cd918ea` | 320 bytes | `m`, linked interval differs |
| 2 | `d587a52` | 320 bytes | `m`, linked interval differs |
| 3 | `a69ad8d` | 316 bytes | `M`, section size differs |
| 4 | `a9fce38` | 320 bytes | `m`, linked interval differs |
| 5 | `2e0a6c1` | 320 bytes | `m`, linked interval differs |
| 6 | `99f4934` | 324 bytes | `M`, section size differs |
| 7 | `872db73` | 320 bytes | `m`, linked interval differs |
| 8 | `684e238` | 320 bytes | `m`, linked interval differs |


## Self-contained diagnostic packet

This packet uses Capstone disassembly as a diagnostic substitute because `tools/diff.py` is unavailable in this cloud setup. Capstone comparisons do not award a match. Only the parent's unmodified `tools/check.py` invocation against a committed-source canonical ARMCC object determines the rank.

### Reproduction and flags

The parent serializes commits and builds. The verification command is:

```sh
. ./development_environment.sh
python make.py eu
python tools/check.py _ZN2al12MemorySystem23createSceneResourceHeapEPKc --object build/eu/obj/lib/al/src/Memory/alMemorySystem.o
```

The canonical compiler is ARMCC 4.1 build 791. Its executable SHA256 is `d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d`. Relevant recorded arguments, taken from the canonical provenance record:

```text
-DVERSION=EU
-DNN_SWITCH_DISABLE_ASSERT_WARNING_FOR_SDK=1
-DNN_SWITCH_DISABLE_DEBUG_PRINT_FOR_SDK=1
-DNON_MATCHING=1
--cpu=MPCore
--fpmode=fast
--apcs=/interwork
--diag_suppress=1608
--arm_only
--no_exceptions
--diag_style=gnu
--arm_only
--no_exceptions
--diag_style=gnu
--signed_chars
--dollar
--force_new_nothrow
--no_rtti
--no_debug_macros
--no_depend_system_headers
-O3
-Otime
--gnu
--split_sections
--force_new_nothrow
--multibyte_chars
--enum_is_int
--signed_chars
--no_rtti_data
--forceinline
--remove_unneeded_entities
--no_debug
--sys_include
--preinclude=Game/project_globals.h
```

The final strict link uses the repository's existing `--inline`; no flags were changed for this function. Early compiler inlining of the same-source helper has been observed. There is no source-closure helper object or guessed standalone helper address in these attempts.

### Target instructions

Existing function start `0x0024F344`, pool start `0x0024F464`, end `0x0024F484`. This is diagnostic disassembly, not build input:

```text
0024F344  push         {r4, r5, r6, r7, r8, lr}
0024F348  movs         r5, r1
0024F34C  sub          sp, sp, #0x28
0024F350  mov          r6, r0
0024F354  mov          r8, #0x800000
0024F358  beq          #0x24f43c
0024F35C  ldr          r1, [pc, #0x100]
0024F360  ldr          r4, [pc, #0x100]
0024F364  mov          r0, sp
0024F368  str          r1, [sp, #4]
0024F36C  str          r4, [sp]
0024F370  bl           #0x243260
0024F374  ldr          r2, [pc, #0xf0]
0024F378  mov          r7, #0
0024F37C  mov          r1, sp
0024F380  str          r2, [sp, #4]
0024F384  str          r4, [sp]
0024F388  bl           #0x290640
0024F38C  mov          r1, r0
0024F390  add          r0, sp, #0x18
0024F394  bl           #0x2905f0
0024F398  mov          r4, #0
0024F39C  add          r0, sp, #0x18
0024F3A0  bl           #0x2910f8
0024F3A4  cmp          r0, #0
0024F3A8  ble          #0x24f43c
0024F3AC  add          r0, sp, #0x10
0024F3B0  bl           #0x2910e8
0024F3B4  mov          r2, r4
0024F3B8  add          r1, sp, #0x10
0024F3BC  add          r0, sp, #0x18
0024F3C0  bl           #0x29107c
0024F3C4  ldr          r2, [pc, #0xa4]
0024F3C8  add          r1, sp, #0xc
0024F3CC  add          r0, sp, #0x10
0024F3D0  str          r7, [sp, #0xc]
0024F3D4  bl           #0x29101c
0024F3D8  ldr          r0, [sp, #0xc]
0024F3DC  mov          r1, r5
0024F3E0  bl           #0x292308
0024F3E4  cmp          r0, #0
0024F3E8  nop          
0024F3EC  beq          #0x24f424
0024F3F0  ldr          r2, [pc, #0x80]
0024F3F4  vldr         s0, [pc, #0x78]
0024F3F8  add          r1, sp, #8
0024F3FC  vstr         s0, [sp, #8]
0024F400  add          r0, sp, #0x10
0024F404  bl           #0x278c30
0024F408  vldr         s0, [pc, #0x6c]
0024F40C  vldr         s1, [sp, #8]
0024F410  vmul.f32     s1, s1, s0
0024F414  vmul.f32     s0, s1, s0
0024F418  vcvt.u32.f32 s0, s0
0024F41C  vmov         r1, s0
0024F420  b            #0x24f440
0024F424  add          r4, r4, #1
0024F428  add          r0, sp, #0x18
0024F42C  bl           #0x2910f8
0024F430  cmp          r0, r4
0024F434  nop          
0024F438  bgt          #0x24f3ac
0024F43C  mov          r1, r8
0024F440  mov          r0, #1
0024F444  str          r0, [sp]
0024F448  str          r0, [sp, #4]
0024F44C  ldr          r2, [pc, #0x2c]
0024F450  mov          r3, #0
0024F454  add          r0, r6, #0xc
0024F458  bl           #0x2911e8
0024F45C  add          sp, sp, #0x28
0024F460  pop          {r4, r5, r6, r7, r8, pc}
```

The pool contains, in order:

```text
0024F464  pointer to ObjectData/GameSystemDataTable (003A2230)
0024F468  SafeString<char> address point (003D9C3C)
0024F46C  pointer to HeapSizeDefine (003A2220)
0024F470  pointer to Stage (003A2250)
0024F474  float 0.0
0024F478  pointer to SceneResource (003A2258)
0024F47C  float 1024.0
0024F480  pointer to SceneHeapResource (003A2268)
```

### Best full-extent C++ checkpoint

Candidate 2 (`d587a52`) is a complete, semantically recovered, canonical NonMatching checkpoint with the exact 320-byte extent and literal pool. Candidate 4 generates the same bytes. The standalone helper name is descriptive source structure, not an assertion of a retail standalone address. Includes and declarations needed by this body:

```cpp
#include <Memory/alMemorySystem.h>
#include <Resource/alResource.h>
#include <Util/alStringUtil.h>
#include <Yaml/alByamlIter.h>

namespace al {
extern "C" void fn_002911e8(sead::FrameHeap** out, u32 heapSize,
        const char* name, sead::Heap* parent, bool enableLock,
        sead::Heap::HeapDirection direction);
extern "C" const char dat_003A2230[];
extern "C" const char dat_003A2220[];
extern "C" const char dat_003A2250[];
extern "C" const char dat_003A2258[];
extern "C" const char dat_003A2268[];
#ifdef NON_MATCHING
static inline u32 calcSceneResourceHeapSize( const char* stageName, u32 defaultSize )
{
        if ( stageName )
        {
                al::Resource* gameSystemDataTable =
                        al::findOrCreateResource( dat_003A2230 );
                const u8*     tableData = gameSystemDataTable->getByml( dat_003A2220 );
                al::ByamlIter table( tableData );
                for ( int i = 0; i < table.getSize(); i++ )
                {
                        al::ByamlIter entry;
                        table.tryGetIterByIndex( &entry, i );
                        const char* stage = nullptr;
                        entry.tryGetStringByKey( &stage, dat_003A2250 );
                        if ( al::isEqualString( stage, stageName ) )
                        {
                                float resourceMb = 0;
                                entry.tryGetFloatByKey( &resourceMb, dat_003A2258 );
                                return resourceMb * 1024 * 1024;
                        }
                }
        }
        return defaultSize;
}

void MemorySystem::createSceneResourceHeap( const char* stageName )
{
        u32 heapSize = calcSceneResourceHeapSize( stageName, 8 * 1024 * 1024 );
        fn_002911e8( &mSceneResourceHeap, heapSize, dat_003A2268, nullptr, true,
                sead::Heap::cHeapDirection_Forward );
}
#endif
} // namespace al
```

### Relevant ABI/layout declarations

- `MemorySystem::mSceneResourceHeap` is a four-byte pointer at +0x0C. Its preceding members are pointers at +0x00 (stationed heap), +0x04 (another heap), and +0x08 (sequence heap). Only +0x0C is accessed by this target.
- `ByamlIter` contains two four-byte pointers: document header at +0x00 and current root/container at +0x04. Its constructors, lookup APIs, and eight-byte size are established independently in accepted code. There is no virtual table.
- `SafeStringBase<char>` has a virtual dispatch pointer at +0x00 and the character pointer at +0x04. The existing class table begins at 0x003D9C34 and the object address point is +8. These temporary SafeStrings have eight-byte size.
- `HeapDirection` is represented as an int: forward +1, reverse -1. The frame-heap helper's parent pointer, lock bool and direction are separate parameters. The helper itself is only an original-address import here, not a claimed implementation.

### Narrow remaining discrepancy and useful next evidence

Candidate 2 has four persistent-register substitutions: retail stageName r5 versus candidate r6, this r6 versus r7, zero r7 versus r8, and default r8 versus r5. Retail entry iterator is at stack +0x10; candidate is at +0x20. The table, string and float slots match at +0x18, +0x0C and +0x08. Retail's successful conversion goes directly into r1 and branches past the default-size move. Candidate updates r5 and later moves r5 into r1 in the common heap-creation block. All branches, callees, float operations, string identities and pool values are otherwise the established operations described above.

Candidate 6 demonstrates that separate caller/helper default merging can generate the desired direct conversion into r1, but it adds a persistent-default transfer and produces 324 bytes. A future attempt should be based on evidence for the original default/result lifetime or the original helper boundary, rather than repeating argument spelling or renaming locals. A genuinely different compiler-phase hypothesis is also testable under the existing canonical source-closure checker, provided it needs no invented runtime addresses or altered toolchain rules.

## Latest-main revalidation

Source committed locally as f4d30e6 against a5041a5091efbf53fdbf99c6950133f2ec41e998, then built with unchanged `python make.py eu`. Canonical command: `.venv/bin/python tools/check.py _ZN2al12MemorySystem23createSceneResourceHeapEPKc --object build/eu/obj/lib/al/src/Memory/alMemorySystem.o`. Exact output: `U -> m: The linked candidate differs from the unchanged original interval.` Latest fetched main a059251507b899d48a6ab58b564cfa8a6d3d360f changes only a notes packet, not the checker or source. No exact claim.
