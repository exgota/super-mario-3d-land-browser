# Voice update: shared audio-state row prerequisite

- Branch `dot/root-2c7adc`; base `6b0e2a1385b814b70344023d9816424edcc7d832`.
- Target `0x002C7ADC..0x002C7D18`, 572 bytes, remains U.
- Missing row at 00430DB0, with neither an exact nor a containing current map row. The target passes it to constructor 2585C4 and scalar query 340640.
- Query 340640 reads through +0x10, proving at least 0x14 bytes for this access. Published root-2c1d20-needs-rows.md and root-2c36b8-needs-rows.md already document larger bounds for the same audio-state object; this note does not replace them with a smaller extent.
- Existing guard row 003F38B8 is present. Object identity/name and final row extent remain integrator-owned.
- No source/build/check attempt; only this short needs-row note is included.
