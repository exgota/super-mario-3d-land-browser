# CoinBox attached-state update

Target: `fn_0030FABC`, EU `0x0030FABC–0x0030FEBC`, 1,024 bytes.
The retained candidate is complete ordinary C++ under `NON_MATCHING`, committed
on current main as `7594e91`. Its canonical project build, compact link, and
export succeeded. The unchanged checker reports `m -> m`: the complete
1,024-byte interval has equal size but different linked bytes. This pass used
all eight permitted candidates and is closed with no exact-match claim.
The final global vector change preserves all 159/159 accepted current-main
functions; the temporary global assignment change was withdrawn. All builds
and checker invocations were performed by the parent.
The authoritative code hash was independently rechecked as
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
No game data, map, rank, boundary, flag, tool, ledger, or STATE edit is included.

## Recovered behavior

On the first state step, clear the mini-wait-animation byte, select AttachMini
or Attach from the attached sensor's player-figure predicate, enable the Body
shadow and disable the Wait shadow, set the Wait shadow scale to (1,1,1), reset
the accumulated distance, and capture this actor's translation.

When the current action finishes, select WaitMini and set the mini-wait byte
for the mini figure; otherwise select Wait. If that byte is set and the mini
predicate has become false, select Wait and clear the byte. These operations
preserve the retail order, including the separate second mini check.

Mirror two owner visibility predicates through separate actor and subordinate
model visibility helpers. If owner byte +0x139 is nonzero, request removal of
the attached item with two false flags and return. Otherwise choose a Body
shadow scale from an independently queried owner state, find JointRoot on the
owner's current model, and update the CoinBox pose from that joint matrix.

Compute the distance from the previously recorded translation. Only when the
item-state predicate is nonzero and the distance exceeds 3 does it contribute
to the accumulated distance. At 500 or more, calculate actor up, start CoinMini
or Coin, and request a coin at translation + up * 100. Decrement the remaining
count only when positive. If that decrement exhausts it, request item removal
and return without resetting the distance or recording a new translation.
Otherwise reset distance to zero, discarding excess distance. All ordinary
exit paths update the previous translation, even when the movement did not
qualify for accumulation. A nonpositive count does not limit emission.

The geometry is expressed as ordinary float arithmetic and sqrtf. No integer
bit reinterpretation, explicit nop, register pinning, assembly, instruction
array, relocation trick, or fabricated alias is used.

## Class and field evidence

The actor factory pair at 0x003B9C00 contains the name pointer 0x003E08BF
("CoinBox") and factory pointer 0x0039857C. That factory allocates 0x80 bytes
and calls constructor 0x0030FFC0. The constructor calls the mapped MapObjActor
constructor and installs vtable 0x003D33C8. The table's init slot points to
0x0030F760, which passes its inline "CoinBox" archive name to the mapped actor
initialization helper. This establishes CoinBox independently of this target.

The same table's receiveMsg slot is 0x0030F534. That function receives its
other sensor in r2, tests it through isSensorPlayer, and stores that exact
sensor pointer at +0x60 before entering the state containing this target.
Consequently +0x60 is HitSensor*, not PlayerActor*. Existing HitSensor layout
and every sensor helper below independently place its owner pointer at +0x28.

Constructor 0x0030FFC0 zeros +0x60, +0x68, +0x78 and byte +0x7C, and copies
Vector3::zero into +0x6C. Init 0x0030F760 reads the placement count with a
local default -1, then stores it at +0x78. Detach routine 0x0030FEBC reads the
sensor owner, stores one incoming flag at +0x7C, clears +0x60, and enters a
separate state. Byte +0x64 is initialized by this target on its first step.
The new header asserts every used offset and the independently allocated
0x80 total size. The field names describe recovered usage; they do not claim
original member spellings or reconstruct unrecovered virtual methods.

Static initializer 0x003805B0 writes the class's four Nerve table pointers.
The attached-state object at 0x003F2670 points to table 0x003BD558. Its execute
entry is 0x0030FAB4, which loads the host from NerveKeeper and falls through to
0x0030FABC. This supports a void state-update contract. The original state
name remains unknown, so the definition keeps its address-based name.

## Called contracts and imports

All unknown functions retain address-labelled symbols. The caller uses no
results from its side-effect helpers. Their void declarations express that
consumer contract rather than claiming a recovered original API spelling.
Word predicates whose original bool/enum spelling is unresolved are declared
as int and consumed only by zero/nonzero tests; no narrowing or normalization
is introduced. The explicit signed-byte reader remains signed char.

- 0x00262B84 follows sensor+0x28, owner+0x74, subsystem+0x40, and reads the
  first figure word. It returns exactly 1 when that word is 1, otherwise 0.
  The Mini action literals and existing player figure layout support the
  semantic mini predicate. This return is independently normalized boolean
- 0x0027CE60 selects among animation-presence and animation-completion
  helpers, returning the selected result or 1 when no animation exists.
  This supports action-completion behavior; current main independently maps
  this as al::isActionEnd, whose genuine declaration is now used
- 0x0027F244 checks the actor action keeper then delegates to the individual
  animation-start routines, including mapped tryStartMclAnimIfExist. It uses
  actor in r0 and the action string in r1. Current main independently maps
  this as void al::startAction, whose genuine declaration is now used
- 0x002728C0 and 0x00272918 obtain actor+0x44's named shadow with 0x0024CC9C.
  They pass 0 and 1 respectively to 0x001D21B0. The former also makes the
  shadow actor appear when its host is alive and it is dead
- 0x00262A20 performs the same named-shadow lookup then copies the three input
  words into shadow+0xA0. Therefore r2 is a three-float scale reference, not
  three floating-point arguments or a scalar
- 0x00262AF8 follows sensor owner+0xA4 and dispatches at virtual offset +0x0C;
  0x00262ADC uses +0x18. The existing PlayerModelHolder interface identifies
  model-hidden and silhouette-hidden predicates. The proposal does not
  rename these unmapped imports
- 0x0027285C/0x00272AEC manipulate actor byte +0x59 and model/shadow visibility.
  0x00262A70/0x00262CC8 apply the same pattern to actor+0x28 then +0x0C,
  establishing that the second visibility pair targets a subordinate actor
- 0x00262A5C reads sensor owner byte +0x139 with LDRSB and returns it directly
- 0x00217E70 dispatches owner model-holder virtual +0x20, then calls 0x00225BE0
  on owner+0x74's +0xAC subsystem with item selector 1 and the two flags.
  That function clears selector bit 1, writes the flags into its message
  data bytes +4/+5, sends the removal message, and clears the attached slot.
  Detach routine 0x0030FEBC is independently reached through that message
- 0x00262A40 follows owner+0x98 and invokes its first virtual entry. This
  predicate's original semantics remain unknown; only its zero/nonzero
  scale selection is reproduced
- 0x00262A04 indexes the current PlayerModelHolder model array: owner+0xA4,
  current index at +0x2C, then pointer at +0x10 + index*4. Return is the current
  model actor pointer. Independent caller 0x0015330C also passes it to
  0x002519A8 and then updatePoseMtx
- 0x002519A8 loads the model actor's +0x28 ModelKeeper and falls through into
  0x002519B0, which resolves the named joint and returns the corresponding
  matrix, or the base matrix when the joint index is negative. Its return is
  a Matrix34 pointer. The existing narrow map interval is not modified
- 0x002CE988 invokes virtual +0x10 on owner+0x74's +0xAC item subsystem.
  Its original predicate name is unresolved; the word result gates distance
  accumulation exactly as in retail
- 0x00269380 checks scene object 10's enable byte +0x50, acquires a coin from
  0x002276D8, applies the supplied position with mapped setTrans, initializes
  it and delegates to 0x0022767C. This is an optional emission request: failure
  of the enable check does not prevent the caller decrementing its budget

Strings dat_003BD4C0 and dat_003BD4C8 are distinct mapped data rows containing
"Body" and "Wait". They are real external string objects, not aliases into
the target. Other strings are ordinary function literals.

The BSS vectors are independently initialized by 0x003805B0:
- dat_0042FC88: (1.0f, 4.0f, 1.0f)
- dat_0042FC94: (1.7f, 0.15f, 1.7f)
Their addresses occur in the target's literal pool and in that initializer.
The source declares them externally; local diagnostic/import registration
belongs to the parent. Their original names and const spelling are unknown.

## Narrow clean vector extension

The mapped add/subtract/scalar routines at 0x0027CB48, 0x0027CB64 and
0x0027CC64 load three floats from each native reference, write three floats
through the output reference, and return without a separate value. Their map
names establish sead::Vector3CalcCtr<float> and nn::math::VEC3 parameters.

The new clean VEC3 declaration has x/y/z at 0/4/8 and size 12. Only the float
Vector3 specialization gains this public storage base and by-value operators
+, -, and *, matching operations demonstrated by this target. Public
inheritance is a type-safe reconstruction choice, not independently recovered
original C++ inheritance spelling. Generic Vector3<T> and Vector2 are unchanged.
The float specialization preserves all existing fields, constructors, static
constants, size, and componentwise operator*= behavior. No removed or external
SDK/sead source was used. The parent captured 118 canonical exact-function
passes before this shared-header change and owns regression verification.

## Interval and verification limits

The map's literal-pool start is 0x0030FE14. Actual instructions resume at
0x0030FE88 after the 100.0f literal at 0x0030FE84. The final reset/copy/return
block through 0x0030FEB8 is part of the function, not disposable pool data.
All 1,024 bytes must remain in the unchanged checker's comparison.

Candidate 1 compiled successfully through the canonical project build. The
strict checker output in `/tmp/mario-latest-coin-check1.log` was:
`U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.`
The 1,016-byte output has the same control-flow and literal-pool sequence. Its
cold-tail translation copy uses LDM/STM where retail uses three explicit
load/store pairs, accounting for the eight-byte length shortfall. Destination
address lifetime, stack layout, and registers also differ. This is diagnostic
comparison, not a substitute for the unchanged checker.

Candidate 2 changes only these two translation assignments to a target-local
inline helper performing x, y, z assignment in order. Implicit assignment and
this helper implement the same observed three-component copy, including
self-assignment. The helper introduces no dummy operations, forced registers,
or instructions. This tests an ordinary source-level copy form while leaving
all shared vector headers frozen for regression checking. Candidate 2 was
committed as `616fbb7`; its canonical build succeeded and emitted 1,024 bytes.
The unchanged checker reported:
`M -> m: The linked candidate differs from the unchanged original interval.`
The parent's 118-function shared-header regression sweep passed 118/118.

Candidate 3 replaces the default float-vector member assignment with explicit
x/y/z assignment returning *this, and restores the two ordinary assignment
expressions in this state. This tests the member-assignment form of the same
observed operation rather than a two-argument free helper. Candidate 2 did
not preserve the retail destination-reference lifetime around getTrans, and
it expanded the first copy differently. No generic-vector or Vector2 code
changes, no new data layout, and no new work are introduced by this member
assignment. Candidate 3 (`8d77653`) compiled successfully and emitted 1,032
bytes. The strict checker reported `m -> M` because its complete extent differs.
It recovers the retail destination reference held across getTrans, r4/r5/r6
save, 0x38 stack frame, and componentwise cold-tail sequence. Its first copy
is still componentwise instead of loading the source value before storing it.

Candidate 4 changes only the first-step assignment to take an explicit
by-value Vector3f snapshot of getTrans's return. The final assignment and
shared headers stay unchanged. This is a value-copy/lifetime hypothesis,
not proof that the original source spelled the operation identically.
It preserves the same three component values without dummy work. Candidate 4
(`ba7e22d`) built successfully, but emitted 1,048 bytes. The checker reported
`M -> M: The complete compiled section, including its literal pool, has a different size from the original interval.`
The explicit snapshot materialized extra stack copies; it is not retained.

The parent checked candidate 3's shared-header assignment change against the
fresh main baseline of 159 accepted functions. It passed 157/159 but broke
PlayerProperty::setUpVec and PlayerProperty::setFrontVec with size mismatches.
That global assignment change is withdrawn. Neither PlayerProperty routine
is modified to accommodate this proposal.

Candidate 5 restores the shared float Vector3 assignment exactly to its
pre-candidate-3 form. CoinBox alone uses a 12-byte, no-extra-state derived
storage wrapper. Its entry initialization delegates to the ordinary base
assignment; frame updates use a componentwise member assignment. The wrapper
and method spelling are explicitly source hypotheses, not new claims about
retail C++ type identity. Both implement the observed three-component value
copy with defined base conversions and no forced instructions or aliasing
casts. It removes candidate 4's explicit stack snapshot. Candidate 5 is
compiled on fresh main `21b` as committed source `5dad30c`. It emitted 1,016
bytes and received strict `U -> M` for complete-interval size mismatch.
The two PlayerProperty regressions are resolved; the parent's full current-main
regression sweep passes all 159/159 accepted functions. The globally safe
vector header is retained unchanged from this point.

Candidate 6 is the last copy-API permutation in this pass. The CoinBox-local
operator= now delegates to the unchanged base assignment, keeping its existing
conventional reference result, while void set performs x/y/z assignment.
The first step uses assignment and the cold tail uses set. This isolates a
source-level assignment/set hypothesis; retail does not prove these API
spellings. There is no newly invented return value, dummy work, or register
forcing. Candidate 6 compiled on fresh main and emitted 1,012 bytes; strict checking
again reported `M -> M` because the complete section size differs.

Candidate 7 retains candidate 2's 1,024-byte source form, the only prior form
with equal extent that did not modify shared assignment semantics. It removes
the CoinBox-only storage wrapper and returns to a plain Vector3f field plus
the target-local componentwise copy helper. This also avoids preserving an
unconfirmed extra type in the partial CoinBox declaration.

Fresh main independently names 0x0027CE60 as al::isActionEnd and 0x0027F244 as
al::startAction. Candidate 7 uses those genuine declarations from
LiveActor/alLiveActorFunction.h and removes the stale C aliases, addressing
compact-link integration without editing any map, tool, or alias logic.
The normalized-boolean proof for isActionEnd is independently confirmed:
its paths reach 0x003307E8, 0x0033070C, or 0x0024EB8C, each explicitly returning
0 or 1. Candidate 7 (`76c8b34`) built, compact-linked, and exported successfully on
fresh main. The strict checker reported `M -> m: The linked candidate differs
from the unchanged original interval.` The complete output remains 1,024
bytes; register allocation, stack slot placement, and copy scheduling differ.

Candidate 8 is the final candidate in this pass. It computes the same vector
norm through an ordinary target-local inline vectorLength(const Vector3f&)
helper returning sqrtf(x*x + y*y + z*z), used directly on the displacement
expression. Retail performs this exact ordered arithmetic and consumes only
its scalar result; the temporary vector is not needed afterwards. This tests
a normal vector-length expression and its scalar-return context rather than
forcing registers or manufacturing extra work. No copy API, layout, import,
or shared-header change is included. Candidate 8 (`7594e91`) built, compact-linked, and exported successfully.
The strict checker reports:
`m -> m: The linked candidate differs from the unchanged original interval.`
It retains the equal 1,024-byte extent. The diagnostic disassembly of the main
path is unchanged from candidate 7, so the length-expression hypothesis did
not resolve the mismatch. Candidate 8 is retained as a meaningful complete
NON_MATCHING reconstruction. This pass stops at eight candidates.

The final source has no CoinBox-only vector wrapper, global operator= change,
padding, fake symbols, instruction payloads, or boundary changes. The accepted
regression baseline remains 159/159 after withdrawing the global assignment.
Outstanding byte-shape differences are destination-reference lifetime, first
and final copy scheduling, stack-slot placement, and float register allocation.
The unknown owner-state predicate names and exact original source spellings
remain unresolved; the implementation preserves their observed ABI behavior.

## Latest packet-baseline verification

The retained source was applied to exact main8ca3fc0f2aa6c8fe0d870cf4992b9f6029286a13 and committed before the normal project build. Its checker/tool files are unchanged in the subsequently fetched af853bf3e12865dbbf1c2c467868fac318d0a7b8. All197 previously accepted functions passed before the proposal batch and again after the clean vector/matrix declarations, restored default vector assignment, published string header, and three guarded proposals were built. Evidence summary:197/197, no new exact credit from these three roots. The full compact build linked/exported after the ordinary provenance-dependent scaffold retry. Later main additions remain subject to main's own intake revalidation.

Current-main check: `python tools/check.py fn_0030FABC --object build/eu/obj/Game/backup/src/MapObj/CoinBoxFollow.o` returned `U -> m: The linked candidate differs from the unchanged original interval.` The final checked source was committed locally as e387a34. Shared math declarations are published as the same additive superset as dot/effect-set-transform, preserving its native MTX34/mul declarations. No global componentwise assignment operator is retained.
