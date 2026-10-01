# Program-link root reservation

Branch `dot/program-link-root`, base `57f902421f6874f132d5ea971d1aa5b224c5a528`.
Reserved target: unnamed root `0x00245D50..0x002476CC`, 6,524 bytes, rank U.
Source reconstruction and bounded validation are in progress. No match is claimed.

The owner dump has the required SHA256
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
The configured module is `lib/CtrSDK`, ARMCC 4.0 build 902, not game 4.1/791.

The observed ABI is a single 32-bit program handle in r0, with no used return.
Independent callers at `0x001D4CA0`, `0x002B57B4`, `0x002B5898`, and
`0x002CBAAC` pass handles previously used by the adjacent attribute-binding
routine. The entry finds a linked-list object through
`(*(0x003E2E40+8)+8)[handle & 511]`, clears its success byte at +0x16,
and validates its primary and optional secondary descriptor references.
A descriptor selected from resource+0x10 has stride 0xE8.

The root constructs uniform locations and dense register sets, resolves
attribute bindings, composes stage outputs, initializes cached state, and
invalidates the active context. Program-link terminology is descriptive; the
original exported API name is not yet independently established.
Shared cached register, light and control offsets agree with the independently
recovered float/integer state and shader-validator proposals.

The complete CFG continues beyond the internal literal pool
`0x00246D08..0x00246D40`, and includes the inline six-entry switch table
`0x002467EC..0x00246804`. Executable code reaches the return at `0x002476B8`;
the final pool is `0x002476BC..0x002476CC`. Neither pool truncates this task.
