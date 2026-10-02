# Operation state: accepted integer receiver prerequisite

- Branch `dot/root-147b08`; base `6b0e2a1385b814b70344023d9816424edcc7d832`.
- Target `0x00147B08..0x00147D40`, 568 bytes, remains U.
- Incoming r0 is retained as the receiver, written at +0x14, and supplied unchanged to named al::startAction at 147B44. This is an object-pointer contract, independently of the unproved return type.
- Accepted Factory group_001414C0 still declares `uint32_t fn_00147B08(uint32_t, const uint32_t*, uint32_t, uint32_t)` and forwards a loaded receiver word from wrapper 147B00.
- The FileSelect cleanup on this base repaired only the separately owned 14AEDC family; it did not repair this declaration. No shared typed receiver interface exists here.
- No integer/pointer adapter, alias or Factory edit. Integrator-owned receiver-contract repair precedes source work.
- No source/build/check attempt. Only this short note is included.
