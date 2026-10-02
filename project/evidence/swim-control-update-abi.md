# Swimming-control update: independent ABI evidence

The proposed root is the existing unnamed EU function 00174024..00174828,
2,052 complete bytes. The frozen source base is de61f9bfed35d5dd22a4ffb24cc98a910ef0b7b0.
The independent 08:19–08:20 UTC queue observation of main269591a54f4bc63fca59383a81b08bd86071b620
still found this row U. That later commit adds two Factory files and map/ledger
records, without modifying prior sources, headers, tools or configuration.
No rank, name, boundary, table partition or shared-header change is proposed.

The source name describes an inferred role. It is not an original class name.
Constructor00174840 stores vptr003CC408 at+0, the supplied context at+4 and
zero at+8. The graph builder allocates0xC and calls this constructor at001AB204.
The table's+0x10 entry points to00174024, with move00173FC4 at+0xC,
setup00173FD8 at+0x14 and teardown00174828 at+0x18. The internal pool at001743F4
is not an end boundary. Executable code resumes at00174420, returns at00174810,
and Swim/SwimWait strings extend to00174828. SwimPaddle is in the internal island.

The pending dot/player-action-builder-source header already owns
PlayerGraphAction_00174840, PlayerGraphActionContext and PlayerGraphConfiguration.
This proposal changes none of them. Its address-qualified ActionView is an
observed memory view: vptr, context pointer and signed counter. ContextView uses
only+0 property,+4 animator and+0x14 input. Builder context construction at001A9778
independently writes those slots. It is not the full Player object.

Main PlayerProperty establishes front at+0xC and up at+0x18. The update and
helper00173F4C independently establish a three-float motion vector at+0x24 and
a turn axis at+0x6C. Translation+0 is not used by this update. The source uses a
separate PropertyView and leaves the public class's opaque fields unchanged.
The vector at+0x24 is called velocity only as an observed role. Helper00173F4C
projects it onto up, and when that dot product is nonzero replaces it by up
multiplied by its original magnitude. This helper remains an original-code import.

Main IUsePlayerAnimator gives several called slots placeholder void signatures.
This proposal does not redefine that class or call through those placeholders.
AnimatorSlots records the observed ABI of this update alone: name08 accepts a
SafeString reference, name18 and name34 accept that reference and return a
boolean, operation2C takes only this, and query30 returns a boolean. The existing
PlayerAnimator vtable at003C91A0 has its object vptr at003C91A8, established by
constructor0014F554. Its corresponding implementations are0014F488,00327FA8,
003280BC,0014F010 and00327EC4. Their argument use and boolean returns corroborate
the update's indirect calls. The first name34 call on paddle input discards its
result; the subsequent name34 call tests it. Both calls are retained, without
inventing an animation-start effect for the first call. These independently
grounded slot signatures are evidence for a future coordinated shared-interface
update, not a silent modification of the pending builder's ABI.

Input slots+0x14 and+0x18 return float in s0; +0x4C and+0x50 return conditions in
r0. Configuration slots+0x354,+0x358,+0x35C,+0x360,+0x368,+0x36C,+0x370,+0x374
and+0x378 return float in s0; +0x364 returns the counter in r0. Names in the new
view describe uses, not a recovered original configuration class. The getter
fn_0026E1DC retains the pending builder's exact declaration returning a pointer
to forward-declared PlayerGraphConfiguration. Only after that call is the result
converted to the separate observed ConfigurationView.

## Direct import contracts

- 0026E1DC: no arguments; loads the global owner's+0x60 then+0x10 and returns the configuration pointer
- 00270844: output Quatf pointer in r0, axis Vector3f pointer in r1, angle in s0; stores xyz=axis*sin(angle/2),w=cos(angle/2). This matches the existing pending dot/root-1dfb44 declaration
- 00258A54: in-place Vector3f in r0 and const quaternion in r1; all four quaternion components and three vector components feed the observed quaternion rotation formula
- 00279ABC: Vector3f pointer in r0; branches to the mapped normalization routine0027CCBC. The return magnitude in s0 is discarded, matching the pending PlayerActorInitSpecial void declaration
- 00173F4C: ActionView pointer in r0; only the context/property slots described above are used
- 0027CD04,0027CC64,0027CB48,0027CB64: the established named Vector3CalcCtr cross, multScalar, add and sub contracts. Output and input VEC3 references are addresses in r0/r1/r2, with scale in s0. The source forward-declares nn::math::VEC3 and does not define or change that shared type

All local dot branch Game/lib occurrences of those C symbols were inspected.
There are no preexisting main declarations of the new unnamed imports. Pending
branches have ABI-equivalent pointer/reference spellings for some explicitly
mangled vector C declarations; this file uses the nn::math::VEC3 reference
spelling already used by emitter-transform. It does not unify or edit those
other proposals. Recovered pointer layouts and ABI calls are established here;
original C++ dynamic identities and a coordinated common header are not claimed.

The source uses no fabricated source helper addresses, raw function bytes,
assembly, removed SDK implementations, compiler changes or shortened intervals.
The only new compilation unit contains the root and inline arithmetic/view
helpers. Constructor/move/setup/teardown, concrete animator implementations,
configuration and input providers remain outside source closure.
