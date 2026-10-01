# Effect-set action update

Target: `fn_001E9B30`, EU `0x001E9B30–0x001EA00C`, 1,244 bytes including
its interleaved literal pool. This is a source proposal, not an accepted match.
Three semantic candidates were compiled through the parent's canonical build.
The strongest retained candidate is 3, with final `NON_MATCHING`-guarded source
committed as `a29faeba4de5169fb2d4d5742e1c74b1864a23ae`.
It emits 1,244 bytes, equal in length to the target, but differs in instructions,
stack layout and literals. The strict checker rejects the unresolved virtual-table imports before comparison. No accepted match or exact byte coverage is
claimed. The final guarded-source canonical rebuild succeeds and receives the
same strict closure rejection. The unchanged EU code hash was reverified.

The lane owns the new `alEffectSetAction.cpp` and `alEffectSetAction.h`, a narrow
extension to the clean `seadSafeString.h`, and the two effect caller includes.
It changes no map, rank, boundary, tool, compiler flag, ledger, or STATE file.

## Recovered behavior

The function selects a 64-character temporary action string. In nonzero mode,
it first creates `StringTmp<64>("Water%s", name ? name : "")`. If an entry's
configuration action label equals the original input name, it returns a deep
copy of that formatted string. Otherwise it looks for the action label stored
through `dat_003F02B4`, whose pointer value names the six-byte `"Water"` string.
If found it returns a temporary formatted directly from that label. In either
mode, the fallback searches the original input name and returns a temporary
formatted from that name, or an empty formatted temporary when absent. The
retail action lookup tests the configuration label for null, but does not add
a null guard for its input argument; the proposal preserves that behavior.

If the selected name equals the current name, the function returns. Equality
is null-safe at this stage: two null pointers compare equal, one null pointer
compares unequal, and two nonnull pointers use `al::isEqualString`.

For entries belonging to the previous action, it requests deletion only if
the handle is live, deletion has not already been requested, and either of
the two configuration deletion-control flags is set. If the set is active,
it emits the corresponding newly selected action entries subject to the same
two configuration flags. Finally it formats the selected name into the set's
embedded current-action buffer using `"%s"`.

## Independent layout and semantic evidence

The constructor at `0x001EA3AC` stores the entry array at +4, count at +8 and
active byte at +0x18. It initializes the embedded string's data pointer to
this+0x28, capacity to 0x40 and virtual pointer at +0x1C. The complete derived
identity of this embedded string is not established, so the proposal represents
only its `BufferedSafeString` prefix and explicit 64-byte backing buffer.

The same constructor indexes source configuration records with a 16-byte
stride and calls the entry constructor at `0x001C5AB4`. That constructor
allocates an eight-byte reference, stores it at entry+4 and stores the
configuration pointer at entry+8. Existing accepted effect helpers independently
read entry+8, configuration+0x0C/+0x0D, array+4, count+8 and active+0x18.
The proposed source asserts all used set, entry and configuration offsets.

`0x0023F45C` establishes the live-handle predicate independently: the reference
object pointer must be nonnull, its saved identifier must equal object+0x0C,
and object+4 must be positive. `0x0023EF90` uses the same validation. Its false
mode sets object byte +0x1CC to one, while true mode invokes immediate deletion
paths. This supports the descriptive `mDeletionRequested` name at +0x1CC.
The source leaves the original identities of both routines address-named.

`0x0023F02C` receives set, entry and an integer/boolean override in r0-r2. It
selects emission setup from set+0x0C/+0x10/+0x14, invokes entry creation at
`0x0023F494`, then applies transform and configuration properties to the live
object. The action update passes zero for the third argument. Its complete
original API name remains unknown.

## Mode ABI and water context

Both established callers load `EffectKeeper` byte +0x11 using LDRSB before
calling this target: `setActionName` at `0x001BFBB8`, and the mode setter at
`0x001BFAE0`. The callee compares the full incoming r2 with zero; it neither
truncates nor normalizes that word. The shared declaration therefore uses
`int`, preserving signed-byte promotion in both callers. This resolves the
old signed-char/int declaration inconsistency without changing the stored
field or guessing its original enum type.

The mode setter is tail-called through the interface wrapper `0x0026EDBC`.
Caller `0x001522C0` passes a boolean from a query using the `"WaterArea"`
literal at `0x00152570`. Caller `0x00161AA4` passes the boolean returned by
`0x0027F11C`, which independently queries `"WaterArea"` at `0x0027F148`.
Thus nonzero mode has independently supported water context. The existing
`mActionChangeMode` spelling is retained rather than renaming a shared field.

## String closure and independent imports

The 64-byte variadic temporary constructor at `0x0027AD3C` has the same
construction and varargs sequence as the already identified 32-byte
`al::StringTmp<32>` constructor at `0x0027BEC8`, differing in capacity and
capacity-specific virtual tables. It initializes pointer+4 to this+12,
capacity+8 to 64, terminates both ends and calls `formatV` at `0x0028AE64`.
That callee reads the buffer/capacity and delegates to the formatted-output
implementation at `0x00109208`, clamping an error/truncated result and writing
the last terminator. The ordinary variadic `format` wrapper is `0x0028E1E4`.

The deep-copy closure is independently present at `0x001090CC`, with capacity
128 rather than 64. It rebinds the destination buffer, rejects self-copy,
clears the first character, virtually assures termination, scans each string
up to 0x10000 characters (returns zero when unterminated), clamps the copied
length to capacity minus one, invokes existing `nnnstdMemCpy` at `0x00292354`,
and terminates when the resulting length exceeds the previous length. The
clean header adds this behavior only; it preserves all fields and virtual
slots. Its new bounded length method is exercised here only for `char`.
The pre-existing map labels `0x001090CC` as a FixedSafeString assignment;
the observed reinitialization stores, not that possibly provisional name,
are the evidence used here.

Proposed function identities for parent review:

- `0x0027AD3C`: `_ZN2al9StringTmpILi64EEC1EPKcz`
- `0x0028AE64`: `_ZN4sead22BufferedSafeStringBaseIcE7formatVEPKcSt9__va_list`
- `0x0028E1E4`: `_ZN4sead22BufferedSafeStringBaseIcE6formatEPKcz`
- Existing `al::isEqualString` at `0x00292308` and `nnnstdMemCpy` at `0x00292354`
  retain their established names
- Other effect functions and the Water pointer data remain address-named at
  existing unchanged map row starts

## Virtual-table boundary blocker

Independent constructor `0x0027AD3C` uses these address points, also observed
in the target:

- BufferedSafeString<char>: address point `0x003DA510`, ABI base `0x003DA508`
- FixedSafeString<64>: address point `0x003DA288`, ABI base `0x003DA280`
- StringTmp<64>: address point `0x003D7AF8`, ABI base `0x003D7AF0`

The known `StringTmp<32>` constructor independently shares the first address
point, but uses `0x003DA260` and `0x003D7AE4` for its capacity-specific types.
The generated ARMCC string constructors use `_ZTV` relocations with addend +8.
None of the three required 64-byte ABI bases is an existing data-row start.
Mapping a symbol to an address point would therefore be eight bytes wrong.
The current rows must not be silently renamed or guessed around. This lane
has not altered any data boundaries or imported a false virtual-table base.
A strict match remains blocked unless the parent independently establishes a
permitted representation of these complete tables or records a separate,
evidence-backed boundary correction under the project hard rules.

## Candidate 1 canonical and diagnostic results

The first `tools/check.py fn_001E9B30 --object
build/eu/obj/lib/al/src/Effect/alEffectSetAction.o` result was:

```text
Source closure rejected: A source helper has no unique canonical C++ definition: _ZN4sead22BufferedSafeStringBaseIcE7formatVEPKcSt9__va_list
```

The parent then recorded the three independently justified ordinary function
names on existing local map rows and retried the unchanged canonical object:

```text
Source closure rejected: An unresolved source helper is referenced by a non-branch relocation.
```

Read-only relocation inspection identifies two unresolved R_ARM_ABS32 entries:
function offset 0x3D8 references `_ZTVN4sead15FixedSafeStringILi64EEE`, and
0x3DC references `_ZTVN2al9StringTmpILi64EEE`. Both carry addend +8, requiring
the unmapped bases 0x003DA280 and 0x003D7AF0 respectively. The external Water
pointer relocation at 0x3EC names an existing unchanged data-row start.
The strict acceptance path stops here pending main-lane data ownership review.
No rank was accepted. No alias, table boundary or checker was modified.

Read-only ELF inspection measures a 1,200-byte function/section against the
1,244-byte target. Its frame is the correct 0xB4 bytes, and the action-selection,
deletion and emission branches are present. It is not byte-exact: temporary
placement, buffered constructor stores, copy-closure scheduling, literal
placement and overall size differ. Candidate 1 has only the FixedSafeString
and StringTmp vtable relocations; the compiler eliminates the BufferedSafeString
literal which retail retains. The current clean constructors write directly
through `buffer`; the independent retail constructors repeatedly use the
stored character pointer. A pointer-based constructor spelling is a future
hypothesis, not a tested fix or permission to change shared source mid-build.

Object SHA256: `82788ef90b381dc9b5f41823f5a878f44e8275b44f690efbddb3c6c82a8d8e25`.
Source SHA256: `014630370ba6a2057a1e5fc40b91f884552ad0fbdd6400632292254ef8481aba`.
Shared header SHA256: `cf85e2fdf874da52deef60189bffa44715077cf57ad646d170236a116cf52703`.
Read-only disassembly and metadata are retained in ignored
`build/dot_effect_action_update/candidate1-disassembly.txt` and
`candidate1-summary.json`. No diagnostic altered any object or target bytes.

## Candidate 2 canonical checkpoint

The parent approved exactly two semantics-preserving constructor changes:
`BufferedSafeStringBase` writes its last terminator through stored `mStringTop`
and `mBufferSize`, and the default `FixedSafeString` constructor writes its first
terminator through stored `mStringTop`. All layouts, slots and character results
are unchanged. The independent variadic constructors at 0x0027BEC8, 0x0027AD3C
and 0x0028E350 support this spelling. Candidate 2 was committed as `fde7c2b`, built successfully by the parent, and
received the same unresolved non-branch relocation rejection. Read-only ELF
inspection measures 1,236 bytes and a 0xBC-byte frame, versus retail's 1,244 bytes
and 0xB4-byte frame. All three independently expected virtual-table relocations
now survive at offsets 0x3E0/0x3E4/0x3E8. Constructor emission improved, but this
is neither a near-match percentage nor acceptance evidence.

Candidate 2 object SHA256:
`598bf515c3d3a324d058af3b87c78aa6f41ec5ba238971b5df826c00c5cc82c5`.
Shared header SHA256:
`8379a909a1c4eedc53ebc8f7b20217c630b9e0efb398814430cb0cadec3f45de`.

## Candidate 3 retained; acceptance blocked

Candidate 3 changes only the Water-label search from a helper argument snapshot
to an explicit loop which rereads `dat_003F02B4` per iteration, matching the
retail load cadence. The other action-name searches and shared header are
unchanged. The parent committed and canonically built this version, then
received the same unresolved non-branch relocation rejection.

The function section is now 1,244 bytes, but equal length is not a match.
The frame is 0x9C rather than retail's 0xB4; the deep-copy scheduling, register
allocation and literal placement visibly differ. Virtual-table relocations
at offsets 0x3C0/0x3C4/0x3C8 target the BufferedSafeString, FixedSafeString<64>
and StringTmp<64> symbols, each with addend +8. Their required bases remain
0x003DA508, 0x003DA280 and 0x003D7AF0, none a current mapped row start.

Pre-guard candidate 3 object SHA256:
`4f7d83888271385c92ef4cb59360f820c05d2c6336a9f4fe268f2832884f27b0`.
Raw function-section SHA256:
`a105eb1f6b8018a4df161ca509cff3c1b696d85791d7293b745dbed169320fe5`.
Its read-only disassembly and metadata are retained under ignored
`build/dot_effect_action_update/candidate3-*`. Three candidates were attempted;
the lane stops at the genuine identity blocker rather than spending the
remaining five attempts without valid data ownership.

The actual project diff command was also attempted read-only:

```text
python tools/diff.py --no_check fn_001E9B30 --format json
Error: Couldn't find in decomp: fn_001E9B30
Make sure to implement this symbol somewhere!
```

It exits 1 because the U target is absent from the compact linked image.
Furthermore, the current differ limits its target range to Pool minus Start,
which would omit this function's code after the interleaved pool. The packet
therefore contains the full target listing, and the ignored diagnostics retain
complete raw object disassembly. Neither limitation was worked around by
editing the oracle or changing target boundaries.

## Shared-header regression checks

The parent's canonical `tools/check.py --object` rechecks all report the
complete source-generated function interval matching byte for byte:

- `fn_001BFAA4`
- `fn_001BFAE0`
- `_ZN2al12EffectKeeper13setActionNameEPKc`
- `_ZN10CourseListC1Ev`
- `_ZN10CourseList6Course17isCourseTypeStageENS0_10CourseTypeE`
- `_ZN2al19tryGetPlacementInfoEPNS_9ByamlIterEPKNS_8ResourceEPKc`

These guard the two shared declaration changes and affected existing header
consumers. They do not establish a match for the new action-update target.

## Final frozen source checkpoint

The parent added only the required `#ifdef NON_MATCHING` guard and committed
`a29faeba4de5169fb2d4d5742e1c74b1864a23ae`, then rebuilt canonically. The final
check again reports `Source closure rejected: An unresolved source helper is
referenced by a non-branch relocation.` Both the entire ARMCC object and the
1,244-byte function section are byte-identical to the unguarded third candidate.
This guard-only rebuild is not a fourth semantic candidate.

Final source SHA256:
`fe85bdb42853d2cd1cc536af433cf7aa36026f0fcae7d7ff969a418109d7a08f`.
The object and section hashes remain the candidate 3 values above. The shared
header remains `8379a909a1c4eedc53ebc8f7b20217c630b9e0efb398814430cb0cadec3f45de`.
The self-contained unanswered packet is `project/pro_requests/001E9B30.md`.
Source and headers are frozen. The next useful step is main-lane data ownership
review; further code generation work must not bypass the unresolved vtables.

## Main-lane intake review

The branch claims no exact match. Main imported this report and self-contained packet as proposals. Shared SafeString/caller/source edits remain on the dot branch while the three required whole virtual-table identities and extents are reviewed independently. Equal1244-byte length is not acceptance; no local exact or functional NonMatching credit is recorded. Further source iterations remain reserved to the dot.
