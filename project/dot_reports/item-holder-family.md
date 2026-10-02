# ItemHolder constructor family on b16e2a0

The seventh source form uses the real ItemHolder class and unchanged
al::ISceneObj base. It compiles to 3,340 bytes. A complete diagnostic link at
the original address has zero byte differences from the entire unchanged
retail interval [0x00275828,0x00276534), including all literal islands.
**This is not yet a canonical match and contributes zero accepted bytes.**
The separate table ownership/boundary repair must land before target grading.

The family branch is `dot/item-holder-family`, based on frozen main
`b16e2a0cc32e0cdacac5ba94feabe67432433391`. The tested source commit is
`120aa056c782c2742b92bb5c21cd7ab51120cba3`. It changes only these family files:

- `Game/backup/include/MapObj/ItemHolder.h`
- `lib/al/include/Item/ItemHolder275828.h` (new private pool layout)
- `lib/al/src/ItemHolder275828.cpp` (new implementation)

The complete source patch applies to the named base without assembly or edits
by the acceptance owner. `git apply --check` passes against the separate
untouched `mario-mainb16` checkout. The source-only patch SHA-256 is
`0bbdd2a218de34a23805cf86cdea846ebd1afd75c50ddd01d7be4641a0c48a40`. No shared ISceneObj/PtrArray header, existing accepted
body, compiler option, map, tool, configuration, STATE or ledger changes.
Parent owns publication; no binary or game asset is included.

## Independent prerequisite and source change

The separate notes-only evidence branch `dot/item-holder-ownership`, commit
`8df182f`, establishes the whole `_ZTV10ItemHolder` allocation at
0x003C488C..0x003C48A0 with address point 0x003C4894. Its report is
`project/dot_reports/item-holder-ownership.md`. It uses separate creators,
scene-holder dispatch, neighbouring constructors and an already-built
ISceneObj-derived native table. That evidence was frozen before this source
form was compiled. The native ItemHolder table produced here independently
has exactly twenty bytes.

The carried six-form implementation from `dot/root-275828` commit
`cd620f97827f8a0ea3c48aeb92c76660f639f613` had an isolated raw dispatch field.
Form7 replaces that field with genuine inheritance and compiler vptr
installation. It retains the established eighteen pool constructors, names,
capacities, free-list layout and imported helpers. The compiler now naturally
combines the vptr and first null-pointer store into the original STM and emits
the original scheduling; no padding, assembly, register directives or custom
compiler flags are involved.

The ItemHolder header now describes nineteen pointer slots, three flags,
untouched padding at +0x53, and words at +0x54/+0x58, with size asserted as
0x5C. Private pool pointers are opaque because the old shared PtrArray header
has a different field order. That unrelated shared container remains untouched.
No existing source on the base includes the ItemHolder header.

The historical six forms remain documented in the original branch report.
Form7 is the only new form. No form8 was needed or run. The change was first
compiled through the normal project build, not a handwritten command.

## Diagnostic result and exact metadata prerequisites

The final canonical project-generated object has SHA-256
`d01bc0c2dd0954867c8b53a947beed487c7d5c56ffc6e596579953c33a64778f`.
The diagnostic linked constructor interval has SHA-256
`c050cbd31750b14295ff146ff0c2f0ca2b85c989f789fde6ad2a67c9af450e0c`.
Both original and compiled extents are 3,340 bytes; zero paired bytes differ.
The compiler is the configured ARMCC 4.1/791, fingerprint
`d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d`.

The diagnostic uses an in-memory copy of map rows. It never passes proposed
boundaries to `check.py` and never writes the canonical map. These are the
separate acceptance-owner prerequisites:

1. Adopt the independently evidenced whole-table partition and identity from
   the ownership report, preserving all target bytes and keeping data rank U
2. Name the unchanged root row as `_ZN10ItemHolderC1Ebbi`
3. Name the existing unchanged 0x0027A7B0..0x0027A7C4 NameRef constructor row
   `_ZN9dot2758287NameRefC1EPKc`, as established by the carried report; no extent
   or rank change is proposed for it
4. Enroll the constructor for canonical grading, build the committed family,
   and run the unchanged checker only after the metadata evidence is accepted

The optional independent name for the four-byte ISceneObj placement method is
not a requirement of the constructor diagnostic. It is not claimed as a new
accepted family member. Constructor imports are the twenty-byte native table,
NameRef constructor 0x0027A7B0, scalar nothrow allocation 0x002932B0, free-list
setup 0x0027A7C4, and existing PtrArray setter 0x0027A81C. No fabricated helper
address is present. The final object has no out-of-line pool helper bodies.
Its only defined functions are the 3,340-byte C1 constructor with a zero-size
C2 alias at the same entry, the two four-byte ISceneObj methods, and a
twenty-byte uncredited ItemHolder name getter. Native getSceneObjName remains a source API method with no
claimed retail callable body; its retail virtual slot is zero.

## Explicit unaccepted table and name-getter closure

The compiler-produced table contains an ABS32 relocation at table offset +8
(address-point slot zero) to `_ZNK10ItemHolder15getSceneObjNameEv`. Its +12
relocation refers to ISceneObj placement, and +16 to ISceneObj init. The native
getter is a twenty-byte weak function returning the existing header's
`"ItemHolder"` literal. Retail slot zero is zero and that literal is absent.
Consequently the generated native table is not a byte-exact reconstruction of
the retail table. The original class's source spelling and eliminated virtual
method are a header-based hypothesis. A raw zero relocation placeholder in the
object does not mean the final native table slot is zero.

Naming the table and fixing its bounds will not repair this data-content
mismatch. The constructor diagnostic isolates only the constructor and imports
the table's independently established base address. It never links or compares
the native table contents or its getter. No retail address is assigned to the
getter, and zero must not be treated as a callable identity.

The existing compact linker has a separate deliberate U-table scaffold route:
`tools/pypstem/stepLink.py::find_scaffold_data_import` requires a unique complete
native table symbol/section, a named mapped U const-data table, and compatible
identity/size metadata. Its dependency traversal then imports the whole table
and stops before traversing its native virtual-method relocations. It records
`table_bytes_accepted=false` and supplies the existing zero-filled scaffold.
A read-only call to that unchanged predicate on the final object and proposed
in-memory row confirms its twenty-byte whole-table eligibility and returns
`table_bytes_accepted=false`. This is a predicate check only, not a run of the
post-enrollment compact projection. That placeholder also does not reproduce
the two nonzero retail default-method slots. This is the established compact-scaffold mechanism, not table acceptance.
The ordinary isolated-function checker likewise resolves the mapped table
address and does not certify the table's own bytes.

Constructor-only intake therefore still requires the acceptance owner to
verify this exact complete-table scaffold projection after the independent
metadata repair, preserve every accepted canonical definition, and run the
unchanged canonical constructor checker. Confirm that the getter is absent from
the retained constructor closure and has no fabricated import or function row.
The current clean build leaves the candidate unenrolled; it is not evidence
that the post-enrollment compact projection succeeds. If the native table is
instead retained, its unlocated getter is an additional source-closure blocker
and must be reported rather than assigned a guessed address.

A byte-complete table, fully reconstructed class, or full native-image claim
remains open. That requires independently explaining/reproducing the original
unused-virtual elimination or recovering genuine callable identity where one
exists. No such elimination experiment, additional source form, or table-byte
patch was performed here. Constructor equality and controlled replay do not
close that work.

## Final-source verification

The final `make.py eu -ca` clean build succeeds, compiling 44 Game, 129 al
and one SDK source, then linking and exporting. Every one of the 667 prior
accepted roots and all 687 selected actual canonical definitions passes the
unchanged checker, with zero failures. This includes the accepted 1,116-byte
item lookup, 32-byte PtrArray setter, generic scene-holder functions, and the
new TU's duplicate accepted ISceneObj init definition. There are zero canonical
candidate checks: the constructor has not been graded.

The first run completed 191 passing definitions before the session was
interrupted. Resume verified every canonical object and source-input provenance
hash before continuing the remaining pairs; no build or source changed across
that interruption. Clean build plus active checking took 564.134 seconds,
including 371.124 seconds after resume, excluding the inactive interruption.
The full report is `build/item_holder_family/preservation667/report.json`,
SHA-256 `9a44d70982d93f31f46742d215f0edf137745f8306579513fdfe2cc8159fc9cb`.
Its `accepted` field remains false and `target_canonical_check_performed` is
false; `prior_preservation_complete` is true.

The final diagnostic constructor passes 656/656 valid-domain original-callee
pairs with zero faults or mismatches, 752 original executable addresses visited,
and 10,490,784 combined steps. A separate noncanonical-ABI stress run passes
123/123 pairs, also with zero faults/mismatches, over 1,967,022 combined steps.
Those 123 inputs contain noncanonical bool register values and remain outside
the C++ boolean argument contract; they do not extend the valid-domain claim.

Both sides execute the whole root and original array/free-list/name helpers,
original nothrow allocator, current-heap wrapper and TLS lookup. Only the heap
virtual allocation callback and explicit TLS fixture are controlled. Results
compare return value, restored stack/callee-saved registers, ordered calls,
all holder state exposed at every allocation, and the whole 131,072-byte fixture.
The full inherited recipe and scope limits remain in the carried replay report.

The final valid report SHA-256 is
`f60912acc57c715009fedf9e141440a7d6eafe5256f8f9608f3494f685182a02`;
the separate ABI report SHA-256 is
`ce77087cad6135672bd4a2acfc1bb36b76ac0db131201478936f3c13dc5ff661`.
The reports live under ignored `build/item_holder_family/` and are not game data
published with this branch.

The complete diagnostic is conditional on the independently proposed table
identity; it does not establish table-byte correctness or final link layout.
The replay remains controlled-input original-callee testing. Heap policy,
concurrent mutation, real actor consumption, end-to-end gameplay and invalid
or overlapping pointers are outside its claim.

Source SHA-256 values:

| File | SHA-256 |
| --- | --- |
| lib/al/src/ItemHolder275828.cpp | edfe1046355f15e381b95bff36b1b48fcaac1847c293ceef6431154e01cfc5f4 |
| lib/al/include/Item/ItemHolder275828.h | 359ecde541a4056493fe1bfcebbabbc10f577313fe8a5b61db90125ba6790a58 |
| Game/backup/include/MapObj/ItemHolder.h | 14eee7da5665fc99f71f2bf9b05c72206d0bb15b5891380b36ead7b4d0fe657b |

The original EU executable retains SHA-256
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
The canonical map remains byte-for-byte b16e2a0, SHA-256
`e77614ca3a67b34978ecbeb88fcaf668b5b0a181c0af62baec2bf5f3925faee5`.

## Timing and handoff

Independent prerequisite investigation started 23:22 UTC on 2026-10-01;
notes were committed before form7. Source-family work began after coordination
at 23:28:53 UTC. Source was committed at approximately 23:29:56 UTC, and the
first normal build plus whole-interval diagnostic was complete at 23:31:19 UTC.
Prior-root checking was interrupted after 191 passing definitions, resumed
around 23:50:44 UTC on unchanged artifacts, and completed before 23:57:16 UTC.
Final replay finished by 00:00:12 UTC on 2026-10-02. The long inactive
interruption is disclosed and excluded from the separately reported 564.134
seconds of build/check execution; it is not disguised as productive time.
The source-family wall interval including that interruption is about 31 minutes.
The acceptance manifest is `project/dot_reports/item-holder-family-manifest.json`;
use it only after its explicit metadata and scaffold prerequisites are met.

Potential candidate coverage is 3,340 complete function bytes. It is entirely
carried pool-source inventory plus the newly established class/table
prerequisite and one new constructor form. Canonical acceptance is pending, so
observed accepted throughput for this work is zero. Do not treat the diagnostic
equality or translated-suffix baseline as acceptance.
