# Spin-state shared-interface preservation

Claims: none.
Base: 31d93976ea7d77ce2bc9fc4580355b7c918fbe78.
Two compiled source forms: interface repair and encoding compatibility revision.
Root 0026C780 remains U/516 bytes with THREE
historical forms; this proposal implements no root and adds no matching credit.

Both alEffectObj and KoopaPillar now use the existing al::StringTmp<128> family.
Its formatting constructor has an external explicit-specialization declaration
in alStringUtil.h. The required ordinary constructor identity at 0028CB38 is
proposed in a separate name-only evidence commit; no C facade or private buffer
survives. No StringTmp helper, destructor or vtable is newly emitted, so there is
no vtable-name change. The source/header constructor contract has no return type.

Bug's three action parameters are const float* values, declared through one
narrow action-parameter import header. Its bundle remains 0x0C and Bug remains
0x88, with the bundle pointer at +7C. The fallback rows are 40 bytes; the proven
four-float readable prefix does not imply a 16-byte owning allocation.

The accepted spin sensor wrapper uses ordinary IUseNerve/Nerve/HitSensor APIs.
Its receiver is the state, with its host LiveActor at +0C. Existing singleton
003F2E90/table 003C0494/execute 0036C334 is referenced, not redefined. The two
adjacent wrappers now include the actual Boolean sendMsg41/sendMsgEnemyAttack
APIs while retaining their void wrapper behavior. Helper 0027A4EC retains the
existing bool declaration shared with GhostPlayer; its original return type
is not newly recovered. Sibling 0019A1A4 and wider anonymous aliases remain open.

Preservation scope: five direct source owners; the full 27-object string-header
closure plus Bug, the spin wrapper and the adjacent wrappers: 30 objects,
146 accepted definitions, 14,812 bytes. A fresh untouched same-tree normal build
and candidate normal build both link with configured ARMCC 4.1/791. Canonical
checks pass all 146 complete intervals for both. Twenty-eight cohort objects
are byte-identical; the other two only replace the constructor import identity.
Every unrelated source object is unchanged. No new emitted definition or table
requires canonical helper admission. Exact input/object/check inventories and
all compiler outputs are retained privately. No miss required an assembly diff.

Common SafeString, LiveActor, Nerve, IUseNerve and collection layouts are unchanged.
The transported collection remains the established LiveActorGroup pointer.
Map ranks/extents, ledger and main are unchanged. No setup code was executed.
Authorized RedPepper 6bd828b7 was inspected for constructor naming; its LICENSE
states GPLv3 and README credits open-ead/sead. No reference body was copied.

The encoding revision replaces the existing 22-byte Shift-JIS KoopaPillar string
literal with ordinary ASCII C++ hexadecimal escapes. The unchanged intake guard
now reads the candidate source successfully while preserving the literal bytes.
The preceding source and proof remain frozen. All 1,099 project objects remain
byte-identical, including KoopaPillar code/data and the literal plus its terminator.

Actual intake-guard blocker: shared_type_violations then reads immutable base
31d93976's Game/backup/src/MapObj/KoopaPillar.cpp through git show and raises
UnicodeDecodeError for byte 0x83 at position 3021. This is an execution failure,
not a normal guard acceptance or rejection. Candidate spelling cannot repair
that immutable-base read. Fetch and merge were intercepted; neither executed.
An operator-side repository text-decoding fix is required before actual intake.
No guard/checker change, bypass or base-history rewrite is part of this proposal.
The report-only correction does not add a compiled source form or root form.

This is a preserved proposal with blocked intake. After the decoding fix, the
operator must approve integrator intake; this report requests no production
intake and grants no approval to merge.
