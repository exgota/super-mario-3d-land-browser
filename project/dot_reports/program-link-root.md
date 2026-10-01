# Program-link root reconstruction

**Integration qualification:** this branch uses the shader-family declarations for shared graphics globals. Their types conflict with the separate state-writer headers if combined directly. See [the common layout evidence and proposed declaration contract](graphics-shared-declarations.md); combined source integration is not claimed.

Branch `dot/program-link-root`, base `57f902421f6874f132d5ea971d1aa5b224c5a528`.
Target: unnamed root `0x00245D50..0x002476CC`, 6,524 bytes, originally rank U.

This is a complete guarded **NonMatching source proposal**, with **zero exact
functions or bytes claimed**. The configured SDK compiler is ARMCC 4.0 build 902.
The complete final section is **5,908 bytes**, 616 bytes smaller than retail;
the root symbol is 5,888 bytes plus 20 bytes of final pool. Resolving the data
identity described below would not resolve this code-generation mismatch.

## Source, ABI and original evidence

`lib/CtrSDK/sources/ProgramLink.cpp` defines the one-argument C ABI root behind
`NON_MATCHING`. `lib/CtrSDK/include/retail/ProgramLink.h` records the observed
program, stage, resource, attribute and context prefixes, with ARM offset
assertions. All source helpers inline into the single root section. Only
source, headers and notes are committed; no map, rank, flags, tool, ledger,
STATE, executable bytes or game data change is included.

The owner dump has the required SHA256
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
The proven compiler executable hash is
`e6aca4ebc280a54065406bcc3a0ae3d9f4d0ac5a4032c3a193adb836c72e70fe`.
No removed SDK or prohibited upstream implementation was used.

The observed ABI is one 32-bit program handle in r0, with no used return.
Independent callers at `0x001D4CA0`, `0x002B57B4`, `0x002B5898`, and
`0x002CBAAC` pass handles also used by adjacent attribute-bind/attach routines.
The entry finds a linked-list program through the current context's 512 buckets,
indexed by `handle & 511`. It clears success byte+0x16 before checking the
blocked byte+0x15, primary reference +c, attachment marker +8, primary resource,
and same-resource identity of the optional secondary reference +0x10.
There is no null-program guard in retail or in this source.

A stage reference has resource +0 and index +4. Resource +0x10 points to records
of stride 0xE8. The separate clean constructor lane for 0x002478D8 corroborates
stage +6/+8 as input/output masks, +c/+10 as their counts, +14 as entrypoint,
+1c..2c as boolean/four integer words, +58/+5c as uniform pointer/count,
+60 as 16 attribute type/name-offset pairs, and +e0/+e4 as string base/length.
Names remain descriptive where the original API identity is unproven.

The complete CFG includes an inline six-entry switch table at
`0x002467EC..0x00246804`, an internal literal pool at
`0x00246D08..0x00246D40`, and the final pool at
`0x002476BC..0x002476CC`. Code resumes beyond the internal pool and reaches
both real returns, at 0x002462A0 and 0x002476B8. No interval was shortened.

## Reconstructed behavior

The source handles all phases of the retail root:

- Collects up to 96 live registers per stage, excluding types 0x8B54/0x8B56,
  and stores a sorted dense register-index list plus zeroed 16-byte register rows
- Allocates 12-byte locations for both stages plus 297 built-ins, rejects a
  combined ordinary-uniform count above 2,048, and frees newly allocated
  register rows if location allocation fails
- Releases the previous locations/register arrays only after new location
  allocation succeeds, replaces their counts and lists, and resets dirty words
- Encodes qualifier, component count, matrix/vector form, stage, dense index,
  name offset and program identity into ordinary locations; constructs all
  built-in location tokens from their enum types
- Applies explicit name-based bindings to 12 attribute slots, then places
  remaining active attributes in the first available slots
- Either copies one stage's seven output descriptors or merges common outputs
  first, followed by secondary-only and primary-only descriptors; computes the
  observed masked secondary output count and combined flags
- Copies default register/byte-enable state, initializes the complete+0x96c
  settings region, and updates geometry/output caches with the retail masks
- Marks the link successful, snapshots/clears change flags, records stage/resource
  identities, and invalidates the current control/context when this program is active

The reusable defaults include global ambient 0.2, first-light color ones,
per-light vector/scalar defaults, material 0.2/0.8 values, enum sentinels,
texture enum 0x6030 and scalar 10.0. These are stores reconstructed from the
retail literals and accesses, not a copied initialization byte array.

The first replay exposed one semantic mistake: full-register writes replace
byte-enable bytes with 15, while partial writes OR their enable bits. Form 2
corrects this. No cosmetic size-tuning forms were spent; two distinct forms
were compiled and compared against the 8-form cap.

## Canonical check and main-owned metadata prerequisite

The final clean `make.py eu -ca` compiles, archives, links and exports, with the
root still rank U and excluded from the compact-link scaffold. The canonical
object has SHA256 `746bbf1ccccaf338a461d30bc3c0aa5bd0750af1edc4675cb5bf94e23a8a7042`.
The final post-clean replay and unchanged canonical checker reproduce the
results below. The normal configured project build emits the committed source object.
With only a temporary local `fn_00245D50` name on the unchanged target row:

    python tools/check.py fn_00245D50 --object build/eu/obj/lib/CtrSDK/sources/ProgramLink.o

The unchanged checker reports:

    Source closure rejected: An unresolved source helper is referenced by a non-branch relocation.

The unresolved ABS32 import is `dat_00420160`, the297-entry built-in uniform
table. The map was restored byte-for-byte after checking. This result is a
source-closure rejection, not a completed canonical byte-diff measurement.
The independently measured 5,908-byte section also differs from the required
6,524-byte complete interval, so no match is claimed or expected from merely
adding the data identity. The source's local frame is 0x344 bytes, versus the
retail 0xEC0 reservation plus its saved argument. Loop expansion, local lifetime,
register allocation and pool layout remain substantial differences.

The separate note `project/dot_reports/program-link-builtin-data.md` proposes
the exact **3,564-byte BSS interval 0x00420160..0x00420F4C** for main review.
Original `_shm_initializeShaderManager` at 0x001064E4 writes precisely IDs 0..296
from source records at 0x003A2F7C, then independently initializes the next table
at 0x00420F4C. A second name-lookup consumer corroborates the 12-byte record
and name-pointer field. No BSS definition, array contents or map edit is supplied.

The two common global declarations use the existing shader-validator forms,
word[4] for 0x003E2E40 and Byte* for 0x003E3154. The FloatState proposal declares
the same symbols with typed pointers; those separate headers still need one
shared declaration contract before they can be included together. This source
includes only its own consistent declaration set.

## Bounded ARM comparison

The complete canonical ARMCC object is linked separately at 0x00600000 only
for behavioral comparison. Retail runs at 0x00245D50 in Unicorn's ARM1176 model,
using the unchanged executable. This diagnostic link is never used as an exact
checker artifact and contains no invented helper address.

The final campaign has **1,769 fixture pairs: 1,763 returning pairs with zero
memory/callback/ABI differences, plus 6 separate paired null-allocation faults**.
The full program, context, control, inputs, prior arrays, newly allocated
buffers and built-in table are compared. Each returning pair also checks
stack-pointer and r4-r11 preservation. All 1,607 executable word addresses in
the static root lie inside visited retail basic blocks; this is block coverage,
not an all-input equivalence or complete conditional-edge proof.

- 85 basic/rejection cases cover both stage counts, active/inactive programs,
  hash-chain traversal, old buffers and missing release callbacks
- 720 cases cover every uniform encoding arm and unknown types, zero counts,
  all three register bitset words, boundary crossings and 96-register ranges
- 700 seeded mixed cases combine duplicate/overlapping uniform ranges,
  attribute collisions/missing names, seven-output merges, geometry modes,
  arbitrary handle bits and varied cached defaults
- 18 callback cases include 12 returning failures/absence cases and the 6 paired
  null-register-allocation faults;6 capacity cases cover 2,047/2,048/2,049 records
- 12 memory-result cases exercise six FPSCR mode settings; 8 cases exercise an
  already-equal geometry cache, including the last previously unvisited block
- **220 additional cases use genuine default registers, byte enables and
  built-in table produced by executing the untouched original initializer**

Thus 1,549 fixture pairs use synthetic table/default state (including 6 faults),
and 220 returning pairs use original-initializer-produced state. The initializer
returns normally and the concatenated register/default-byte/table data hashes to
`c64dce6e8b8e62e18b16e1f1ec988bf7c08366031f39c2cce0d2891016c5f737`.
No extracted data is committed. Initialization of other game systems is not
covered by these cases.

The original strcmp, zero-fill entry 0x0028D1F0, __aeabi_memcpy4 and __rt_memcpy
execute their untouched retail bodies. Only allocation/release callbacks are
modeled: allocation returns deterministic distinct aligned storage or null;
release records its arguments. Their call order, argument values and allocation
sizes agree. The actual application allocator, freed-memory effects, callbacks
that mutate/re-enter the linker, concurrent changes, and malformed aliasing
between program/context/resources/arrays are outside this validation.

Register ranges are bounded to 0..95 and bindings to 0..11; merged output lists
fit seven words. Retail can write beyond these temporary/object arrays for
malformed input, and it zero-fills a null pointer after failed nonempty-register
allocation. The proposal preserves these assumptions and adds no invented
fallback behavior. Six agreeing faults do not establish equivalence for every
invalid input or allocator implementation.

The complete replay script and repository-relative build/link recipe are now
included in `project/dot_reports/program-link-replay.md`. The packaged recipe
was executed successfully after the source checkpoint, reproducing all 1,769
fixture pairs while preserving the tested source and canonical object bytes.
Generated replay files, frozen earlier failures/results, diagnostic binaries and
canonical-check logs remain under ignored `build/program_link_validation/`.
No owner binary or extracted initialization data is included in the branch.
