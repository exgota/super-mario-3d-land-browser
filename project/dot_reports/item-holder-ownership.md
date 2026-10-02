# ItemHolder ownership prerequisite for root 0x00275828

This is an independent data/layout evidence proposal, not a new source form or
an exact-match claim. Investigation ran on 2026-10-01 from 23:22 to 23:28 UTC
against frozen main `b16e2a0cc32e0cdacac5ba94feabe67432433391`. The branch is
`dot/item-holder-ownership`. Only this report changes. No compiler was run and
no map, tool, configuration, source, header, binary, STATE, or ledger was edited.

## Finding and limits

The evidence supports the existing public reconstruction's `ItemHolder :
al::ISceneObj` relationship. Its primary base occupies four bytes: a vptr at
offset zero. The holder's first data member starts at +4, its allocation size
is 0x5C, and the interface has three virtual positions with no virtual
destructor. A normal derived-class constructor is therefore a justified next
source hypothesis for the remaining prefix mismatch.

The whole holder virtual table is `[0x003C488C,0x003C48A0)`, twenty bytes, with
address point `0x003C4894`. The current map row starting at 0x003C4894 is an
address-point slice: it omits this table's eight-byte prefix and includes the
following table's eight-byte prefix. It is not a whole-table allocation.

This conclusion uses independent creators, consumers, neighbouring constructors,
the existing source interface, and an already-built ARMCC object. It is not
inferred solely from root 0x00275828's literal. `_ZTV10ItemHolder` is an inferred
C++ reconstruction identity, not a recovered debug symbol.

The first virtual position is zero in retail. There is no retail `ItemHolder`
string in the executable and no callable name-getter body to identify here.
The current header's inline `getSceneObjName()` can remain source API evidence,
but must not receive an invented function address or exact-byte credit. There
is also no evidence here for an out-of-line ISceneObj constructor, a separate
base-construction table, a virtual destructor, or any additional base state.

## Independent creator and consumer evidence

The three direct branches into 0x00275828 are at 0x00123878, 0x00125D04 and
0x0017EFD0. Their containing mapped roots are 0x001235E4, 0x00125BC4 and
0x0017EE20. Each allocates exactly 0x5C bytes before the call, passes the
unchanged returned object pointer to the already accepted scene registration
function at 0x001CB48C, and supplies scene-object ID 10. The first caller
computes the two flag inputs; the other two pass zero/zero and third input 2.
The already completed root report records the detailed boolean-domain evidence.

Registration 0x001CB48C forwards the same pointer to the accepted
`SceneObjHolder::setObj` at 0x001CB4AC, which stores it directly in the indexed
pointer array. There is no base adjustment. The accepted placement loop at
0x001CB4B8 loads the object's vptr and calls virtual slot +4, passing the
ActorInitInfo reference as r1. The accepted create path at 0x001CB508 calls
virtual slot +8 after placing a newly created object in that array. These
dispatch positions independently establish the interface's callable layout.

The retail pointer at address-point +4 is 0x001EBC54, a complete four-byte
`bx lr` row. The pointer at +8 is the accepted
`al::ISceneObj::initSceneObj` at 0x001EBC50, also four bytes. Together with the
three-method public ISceneObj header and placement dispatch, this supports
the previously unnamed 0x001EBC54 identity as
`_ZN2al9ISceneObj26initAfterPlacementSceneObjERKNS_13ActorInitInfoE`. Its current
U/f row bounds `[0x001EBC54,0x001EBC58)` need no change. Merely naming it gives
no exact credit.

Each holder creator also calls 0x00275800. That helper allocates 0x18 bytes,
constructs a CoinCharger through 0x0027A83C, and stores the result at holder
+0x4C. Separately, 0x00277594 retrieves scene ID 10 and calls 0x002775B0, which
loads holder +0x4C and tail-calls the NerveExecutor update at 0x0027B278. This
joins scene ownership and the historical ItemHolder/CoinCharger layout without
depending on the constructor's table literal.

## Whole-table extent and neighbouring ownership

The verified original executable has SHA-256
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
The relevant five-word table records, expressed as semantic positions, are:

| Whole allocation | Address point | Prefix words | Name position | Placement position | Init position |
| --- | --- | --- | --- | --- | --- |
| 0x003C4878..0x003C488C | 0x003C4880 | zero, zero | zero | 0x001EBC54 | 0x001EBC50 |
| 0x003C488C..0x003C48A0 | 0x003C4894 | zero, zero | zero | 0x001EBC54 | 0x001EBC50 |

The first allocation belongs to a distinct object, not the holder's base
construction phase: independent creator 0x0011A8B0 allocates 0x28 bytes and
calls constructor 0x0011B998. That constructor loads literal 0x003C4880 at
0x0011BA18 and installs it at object +0. Its unrelated buffer/atomic state
also differs from the 0x5C holder. The preceding actor constructor at
0x0011B530 installs primary address point 0x003C47D0 and secondary point
0x003C4870 into subobject +0x60. The latter's final callable slot is at
0x003C4874. The following zeros at 0x003C4878/0x003C487C therefore belong to
the next object's prefix, not to a fourth ISceneObj method.

The following actor constructor at 0x00279C0C calls the known LiveActor
constructor, loads literal 0x003C48A8 at 0x00279C7C, and installs that primary
address point using the `stm r0,{r4,ip}` at 0x00279C3C. Its table begins with
the accepted LiveActor virtual positions. The zeros at 0x003C48A0/0x003C48A4
are its primary prefix. They do not extend ItemHolder's table.

The two scene-object address-point constants each have exactly one aligned
literal occurrence in the executable: 0x003C4880 at 0x0011BA18 and 0x003C4894
at 0x00275C58. This uniqueness is a cross-check, not the ownership argument.

An existing project-built GhostPlayerRecorder object supplies independent ABI
shape evidence without another compile. That class directly derives from the
same unchanged ISceneObj header, adds no virtual positions, and overrides its
name and init methods. Its emitted `_ZTV19GhostPlayerRecorder` object and
complete native const section are twenty bytes, with relocations at +8 to
its name method, +12 to ISceneObj placement, and +16 to its init override.
The constructor's table reference carries addend +8. Thus the source ABI
agrees with the independently observed retail address points and extents.

The inspected object was
`mario-root-275828/build/eu/obj/Game/backup/src/Player/GhostPlayerRecorder.o`,
SHA-256 `a8ad6f6f2d72e0a5b69ca25d47d8fea98a65596638ae9be35fba231e6d9e3ec8`.
Its provenance records `tools.pypstem.stepBuild`, EU, ARMCC 4.1/791,
stable source inputs, and that object hash. The compiler hash is
`d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d`.
This object observation is not a claim about the separately mapped
GhostPlayerRecorder table's current extent or rank.

## Separate metadata proposal for the acceptance owner

No metadata patch is made on this branch. A minimal separate hard-rule-2 repair
can preserve the complete existing union `[0x003C47D0,0x003C48A8)` while
repartitioning its three current U/dc rows into four U/dc rows:

| Proposed interval | Proposed identity |
| --- | --- |
| 0x003C47D0..0x003C4878 | preceding anonymous actor-table slice, still unnamed |
| 0x003C4878..0x003C488C | whole neighbouring 0x28-byte object's table, unnamed |
| 0x003C488C..0x003C48A0 | `_ZTV10ItemHolder`, whole twenty-byte table |
| 0x003C48A0..0x003C48A8 | following actor's anonymous eight-byte prefix |

The next row at 0x003C48A8 stays unchanged. The total affected union remains
216 bytes, contiguous and nonoverlapping. No instruction boundary or target
byte changes. No other class table is named, and no rank changes are proposed.
The partial anonymous neighbours remain ineligible as named whole-table
imports. The optional independent naming of the placement method at
0x001EBC54 is described above.

## Apply-clean source-family plan, pending coordination

Use the exact final pool-construction family from
`dot/root-275828` commit `cd620f97827f8a0ea3c48aeb92c76660f639f613` as carried
inventory, against frozen main b16e2a0. Its two additive source/header files do
not exist on this base. One owning lane should prepare and submit the complete
final family patch, so acceptance never requires the root to assemble source.

The supported hypothesis is to integrate the actual `ItemHolder` class using
the existing ISceneObj inheritance, instead of the isolated Holder's explicit
dispatch word. Keep the ISceneObj header unchanged. Update only the dedicated
ItemHolder header to its observed fields: nineteen pointers at +4..+0x4C,
booleans at +0x50/+0x51/+0x52, untouched padding at +0x53, integer input at
+0x54, and limit at +0x58. The constructor takes the supported `(bool,bool,int)`
source parameters and relies on compiler-generated vptr installation. Preserve
all eighteen pool constructors, labels, nested storage, and out-of-line helper
imports from the completed candidate. The expected native table must be checked
for twenty-byte whole-section ownership before metadata intake.

Do not reuse or repair the historical shared `sead::PtrArray` storage layout in
this family. Its current header has capacity before size, whereas the verified
pool view has size before capacity. Keep the isolated pool representation or
use private opaque pointers in the ItemHolder header. A shared PtrArray change
would need a separate owner and a much wider preservation gate. Likewise, no
shared ISceneObj constructor/body change is supported or needed.

Affected source ownership is the new ItemHolder implementation/private pool
header plus `Game/backup/include/MapObj/ItemHolder.h`; no other current source
includes that ItemHolder header. The two accepted nearby roots remain separate
translation units: item-name lookup 0x00277EC4 (1,116 bytes) and PtrArrayImpl
setter 0x0027A81C (32 bytes). Recheck both. The emitted ISceneObj default-init
definition at 0x001EBC50 is already accepted and must remain exact; placement
0x001EBC54 is an unaccepted four-byte possible companion, not assumed credit.
The generic scene-holder registration/create/placement functions above are
accepted ABI witnesses and should be included in a focused preservation pass.
Final intake still requires every prior accepted root and actual canonical
definition, the clean project build, source/helper closure, and canonical
checking of the complete constructor including all pool islands.

Only after coordination and the independent table repair should the owning lane
start a bounded new source pass. Retain the six earlier forms and their failures;
this evidence investigation consumes no compiler form. The carried candidate
remains 3,344 versus 3,340 bytes, with 656 valid-domain original-callee pairs
and a diagnostic translated suffix of 3,200 bytes. These prior measurements
suggest useful potential yield but do not predict successful canonical matching
or establish throughput. No prefix form, resulting size, preservation gate,
or whole-root replay has been tested by this branch.

## Source witnesses and reproducibility

All source hashes below are from frozen main b16e2a0:

| Path | SHA-256 |
| --- | --- |
| Game/backup/include/MapObj/ItemHolder.h | 3628dfb3bc6587204b94feae5d34f02fbad3bd1ccc3a7b0b10f2fcd51fe4e272 |
| lib/al/include/Scene/alISceneObj.h | 31b797683621be67385e24f74d4708867edc94db2e749a791c2c77baf3bb7a41 |
| Game/backup/include/Player/GhostPlayerRecorder.h | 73eadad586f57f5edd0007eebcf7335555a53334815104b69fd2d6a30f9eae6d |
| Game/backup/src/Player/GhostPlayerRecorder.cpp | 06ed22e6a70893a089d12d488660b7f38e0b73cc91df3e66755b3d7360500760 |
| lib/al/src/Scene/alSceneObjHolder.cpp | 999ad0df20e923b02197d81dfe76b99fff79efdcc894dd1c4bd02c26ed075be8 |
| lib/sead/include/container/seadPtrArray.h | 2c3f93c561da6f13edefd2a44ec318d035885cbeb62bf3c1e8f4bc204f7ab632 |

For a read-only check, use the owner's fingerprinted private executable loaded
at 0x00100000. Decode the creator, scene-holder, neighbour and coin-charger
intervals named above as ARM, and inspect words in 0x003C4878..0x003C48A8.
Inspect `.constdata__ZTV19GhostPlayerRecorder`, its symbol size and relocation
offsets in the existing object with pyelftools or the normal object dumper.
Do not interpret 0x00275C58 as the constructor's instruction end: it is an
interior literal island, and the existing final root report covers all islands.

The note is the complete deliverable. Dependent compilation is paused for lane
coordination; publication and metadata edits belong to the parent/acceptance
owner. The eight active Pro reservations remain untouched.
