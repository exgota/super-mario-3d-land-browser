# AquariumSwimDebris family source evidence

Frozen base: b16e2a0cc32e0cdacac5ba94feabe67432433391. Final source SHA256: 1dc37a6c417ff58af734c449915fb41689af1ec04db8ce9558fc37a99b27adc2.

The proposal covers init00186CC8..00186D44 (124 complete bytes, pool at00186D28)
and constructor00186D44..00186D70 (44 complete bytes, pool at00186D6C).
Both complete intervals equal retail under ARMCC4.1/791 and894 in scratch.
Only canonical project build/check acceptance may add exact credit.

The only source change replaces two typed StageSwitch calls with existing opaque
C entry names fn_00280538 and fn_0027FAB8. Their declarations reuse the receiver
prototypes in accepted TransparentWall.cpp, whose base source SHA256 is
1b5ef45491883ba345834bc11d6642a8f72f230549e40ccb4b2c834e3933ad01. Accepted retail TransparentWall::init001674AC..00167574
independently calls both entries. Aquarium passes actor+0xC to00280538, agreeing
with the established IUseStageSwitch base, and the actor to0027FAB8. The first
provider dispatches receiver virtual slots; the second adjusts the LiveActor
receiver by0xC and returns0 or1 after choosing appearance. Existing provider
rows, names, bounds, rank and type remain unchanged. No provider body is proposed.

The final objects preserve both previously accepted emitted definitions, the
48-byte Appear callback and4-byte inherited executeOnEnd, with their complete
original intervals under both compilers. All15 other previously emitted function
symbol definitions also preserve sections, normalized relocations and binding.
Init is the sole changed definition, and only its import names change.

All26 actual used inputs/configuration except this source equal the frozen base.
Headers and preinclude were copied before compilation and remain unchanged.
The24-byte local static initializer is unchanged and remains outside enrollment.
The enrollment patch changes only these two U ranks toM for root's canonical gate.

One original-source form compiled successfully but failed import isolation because
its typed names lacked map identities. The final form compiled successfully under
both builds and matched immediately. Three physical compiles, zero compile errors,
one unsuccessful diagnostic form. The first aborted driver's whole observed window
is0.708843 seconds; its isolated compiler timer was not serialized. The final paired
script spans1.188684 seconds, with0.405947 seconds in compiler calls.

Expected-selection score was448 accepted bytes/hour from0.80*168/0.30 hours,
with zero speculative downstream bytes. This is an estimate, not measured throughput.
The score included investigation, implementation and root intake. No canonical
acceptance or end-to-end lane rate is claimed in this evidence.
