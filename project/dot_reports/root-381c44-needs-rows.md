# Static parameter initialization: missing object rows

- Branch `dot/root-381c44`; base `6b0e2a1385b814b70344023d9816424edcc7d832`.
- Target `0x00381C44..0x00381E80`, 572 bytes, remains U.
- Missing 0042FA08: direct scalar/vector stores establish a minimum 0x2C-byte span.
- Missing 0042FA34: constructor 1A6344 initializes a prefix and two string members at +0x14/+0x40. Their helper 252BF8 writes a terminator at member +0x2B, establishing a minimum 0x6C-byte object span.
- Missing 0042FAA0: constructor 1A1580 initializes a prefix and string members at +0x18/+0x44; the same terminal write establishes a minimum 0x70-byte span.
- None has an exact or containing map row. Keep these separate objects; full class names and any larger extents remain unproved.
- The 17 destination words from 003F2420 through 003F2460 each have their own four-byte row, and referenced strings have rows. This establishes individual coverage, not a recovered whole-array identity.
- No source/build/check attempt, data contents or disassembly. Integrator owns adding/naming rows; payload is this note only.
