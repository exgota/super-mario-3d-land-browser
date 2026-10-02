# State presentation: shared vector argument prerequisite

- Branch `dot/root-2f5f1c`; base `6b0e2a1385b814b70344023d9816424edcc7d832`.
- Target `0x002F5F1C..0x002F6154`, 568 bytes, remains U.
- At 2F5F50, the target calls 273050 with the receiver's +0x60 presentation pointer and a mapped vector argument.
- Accepted Factory fn_002D798C declares `fn_00273050(Presentation*, const Vector3&)` using its anonymous-namespace types. No coherent public declaration exists for reuse by this target.
- The scalar vector layout alone does not make independent C++ types compatible. Shared interface repair remains integrator-owned; no new private alias or cast adapter was introduced.
- The accepted root wrapper's broad void-pointer return contract is not independently resolved by this note and is not used as the blocker.
- No source/build/check attempt or exact claim. Only this note is included.
