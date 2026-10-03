# Vector3<float>::ex data identity

Frozen main: `40e0f982325cbd91ae5d42f568f5934a31ec83a5`. This evidence-only proposal adds one complete twelve-byte BSS row, `004305C8..004305D4`, with symbol `_ZN4sead7Vector3IfE2exE`, type `db`, rank U and blank Pool/Section fields. Every prior map line remains byte-for-byte unchanged. No function source, prior rank, ledger, compiler setting or function boundary changes. Function credit is zero.

## Independent evidence

The owner EU executable has SHA-256 `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. Its vector static initializer at `00383870`, already mapped as `__sti___14_seadVector_cpp`, is selected by startup registration entry `003A0E44`. The startup dispatcher and relative entry resolve independently of the target consumer.

The driver reran the original straight-line initializer decoder and scalar register/store interpretation for this proposal. It rejects unrecognized operations. Address load `00383B10` uses pointer literal `00383D84` for `004305C8`. Stores at `00383B14`, `00383B18` and `00383B1C` initialize three consecutive four-byte float components to (1, 0, 0). The next independently initialized vector begins at `004305D4` and has (0, 1, 0); the following vector at `004305E0` has (0, 0, 1). The latter already has the complete twelve-byte ez map row.

The three component stores and immediate next vector establish the complete ex extent through `004305D4`, without invented padding or an enclosing allocation. These addresses are beyond the executable's file-backed range. Values come from the startup writes, not from reading nonexistent BSS file bytes.

The accepted `lib/sead/include/math/seadVector.h` has x, y and z scalar fields and declares `static const Vector3 ex`. `nn/types.h` defines f32 as float. The accepted clean-room `lib/sead/README.md` already identifies `004305C8` as ex. This public reconstruction and independent original producer establish the identity and mangled spelling. They do not recover an original debug symbol or prove the writable BSS region is hardware read-only or never changes after startup.

## Scope and dependency

Fetched `dot/root-316014`, report `project/dot_reports/root-316014-needs-rows.md`, stops before source construction on missing ex and ey coverage. Its function interval is 532 bytes. This proposal supplies ex only; the independently evidenced ey row is a separate pending proposal. Neither prerequisite means that the report-only target has source or matches. No target implementation is included.

No existing containing row or equivalent queued proposal was found before preparation. All original map lines remain unchanged, and the working owner map was unchanged by verification. Integrator acceptance and its usual regression gate remain required.
