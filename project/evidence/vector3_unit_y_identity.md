# Vector3<float>::ey data identity

Frozen main: `9f3473406351714c2c0aec8c6927d77c3bb6b9c6`. This evidence-only proposal adds one twelve-byte BSS row, `004305D4..004305E0`, with symbol `_ZN4sead7Vector3IfE2eyE`, type `db`, rank U and blank Pool/Section fields. Every pre-existing map line is byte-for-byte unchanged, including the adjacent ez, ones and zero rows. No function source, rank, ledger, boundary, compiler or oracle change is proposed. Exact function credit is zero.

## Independent producer evidence

The original owner EU executable has SHA-256 `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. Its registered vector static initializer is `00383870`, already named `__sti___14_seadVector_cpp`. The startup dispatcher at `003A0DA4` interprets four-byte relative entries; entry `003A0E44` selects this initializer.

The driver independently reran the helper's straight-line decoder and scalar-register/store interpretation. It verifies the retained float zero and one inputs, exact address loads, all component writes, neighboring vectors and the startup registration. An unexpected instruction is a failure, not silently accepted.

| Object | Start | Exclusive end | Components | Address load | Write sites |
| --- | --- | --- | --- | --- | --- |
| ex, neighbor only | `004305C8` | `004305D4` | (1, 0, 0) | `00383B10` | `00383B14`, `00383B18`, `00383B1C` |
| ey, proposed row | `004305D4` | `004305E0` | (0, 1, 0) | `00383B20` | `00383B24`, `00383B28` |
| ez, existing row | `004305E0` | `004305EC` | (0, 0, 1) | `00383B2C` | `00383B30`, `00383B34` |

The ey address comes from the literal at `00383D88`. Its first store writes two consecutive four-byte components, zero and one; its second writes zero at offset eight. The immediate next independently initialized vector starts at the exclusive end, which supports the complete twelve-byte extent. The preceding ex extent ends at the ey base. No enclosing padding or one-word placeholder is invented.

The accepted `lib/sead/include/math/seadVector.h` declares Vector3<T> with only x, y and z scalar fields and `static const Vector3 ey`. `nn/types.h` defines f32 as float. The accepted clean-room `lib/sead/README.md` already records this address as ey. This joins the original producer evidence to the accepted public reconstruction, independently of any desired consumer relocation. The mangled spelling follows that reconstructed declaration; the stripped executable does not establish original debug-symbol spelling. Const is the public API contract, not a claim that the writable BSS region is hardware read-only or never written after startup.

The address lies beyond the file-backed executable range. Its values are established by initialization, not read as nonexistent file bytes. Type db follows the adjacent accepted BSS rows.

## Dependency and limits

Five fetched report-only proposals identify this prerequisite: root-18ab40, root-316788, root-316014, root-158d70 and root-186a00. Their target intervals total 2,724 bytes, which are not source deliveries or exact credit. Root-316014 also needs ex; root-316788 retains separate interface blockers. This proposal adds ey only and does not reconstruct any of those hard functions.

No equivalent current row or pending/held row proposal was found immediately before preparation. The earlier low-confidence zero association in `docs/facts/00268048.md` is corrected to this independently established ey identity. No other fact, source or neighboring missing row is changed. Integrator acceptance and the usual regression gate remain required.
