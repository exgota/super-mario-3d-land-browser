# Directional motion update: observed local ABI and arithmetic

The unnamed EU root 001A3F08..001A468C occupies 1924 bytes. Its code ranges
001A3F08..001A42B0 and 001A42C8..001A4688 contain 474 instructions; embedded and
terminal literals remain unchanged. The descriptive filename does not claim an
original class or API name.

Entry r0 is a controller. Observed fields are motion pointer +10, input interface
+14, reset-trigger interface +18, optional listener +20, blend interface +28,
frame count +38 and persistent fast-frame count +40. The motion view contains
forward vector at +C and velocity at +24. A helper obtains the configuration
interface. Virtual slots retain the exact observed byte offsets; each query
reloads its object's vtable rather than caching values that the original re-queries.
Local assertions verify the field positions. Other headers and families stay unchanged.

The root optionally resets motion, normalizes its forward vector, and constructs
an orthogonal up/right basis through the original cross and normalization imports.
It projects velocity into that basis and chooses a friction identifier according
to an input-interface query. Fast-frame state increments above 0.95 times a
queried base speed; below the threshold it resets only while still below ten.
Counters retain unsigned wrapping behavior.

Two direction tests compare unsigned float bit patterns against 0xBDCCCCCD and
0xBE31D0D4. They are not rewritten as ordinary signed floating comparisons. The
second occurs after normalizing the input and taking its dot product with forward.
The source preserves these exact bit predicates and the control-flow placement
of friction and stopping decisions.

The active-input path removes the up component, normalizes desired direction,
and computes a target from repeated virtual float queries and input magnitude.
Acceleration chooses between separate unsigned frame-duration queries. Its result
is clamped according to the observed conditions, then a speed-dependent turn
factor interpolates two further queries. Repeated calls remain distinct; tests
include configuration methods returning different values on successive calls.

Quaternion construction is an imported operation. The root's inlined quaternion
application is reconstructed directly as ordinary scalar C++, retaining the
original multiplication/addition/subtraction grouping, intermediate signs, and
component order. It normalizes the rotated forward vector before storing it.
The root then applies vertical acceleration and a separately re-queried terminal
limit, recombines the three velocity components through original vector helpers,
projects planar velocity, computes its magnitude, notifies the optional listener,
calls the two final update helpers, and increments the frame counter.

The known cross, multiply-scalar and add identities use existing sead/nn math
names with fresh minimal declarations verified by the map and calling convention.
No removed-library implementation is restored. Other imports retain the original
address-derived names. All virtual dispatch and scalar operations are ordinary
C++, with no instruction arrays, inline assembly or compiler-flag changes.

The initial local up-vector aggregate caused the compiler to generate an extra
read-only initializer. The unchanged closure checker rejected that relocation.
Explicit scalar stores express the original local construction and remove that
initializer. The resulting source reaches the canonical size test but remains
NonMatching. Existing lib/al ARMCC 4.1/791 placement is a build carrier, not proof
of this root's original compiler configuration.
